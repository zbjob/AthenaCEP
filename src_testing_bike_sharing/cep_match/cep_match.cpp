#include "../Query.h"
#include "../PatternMatcher.h"
#include "../EventStream.h"
#include "../_shared/PredicateMiner.h"
#include "../_shared/MonitorThread.h"
#include "../_shared/GlobalClock.h"

#include "../freegetopt/getopt.h"

#include "../Bike_event.h"

#include <vector>
#include <chrono>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <queue>
#include <string>
#include <random>

using namespace std;
using namespace std::chrono;

//static time_point<high_resolution_clock> g_BeginClock;

time_point<high_resolution_clock> g_BeginClock;
long int NumFullMatch = 0;
long int NumHighLatency = 0;
long int NumPartialMatch = 0;
long int NumSheddingPM = 0;
uint64_t ACCLantency = 0;

default_random_engine m_generator;
uniform_int_distribution<int> m_distribution(1,100);


inline void init_utime() 
{
	g_BeginClock = high_resolution_clock::now();
}

inline uint64_t current_utime() 
{ 
	return duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count();
}

class CepMatch
{
public:
	CepMatch() : m_Query(0), m_DefAttrId(0), m_DefAttrOffset(0), m_NextMinerUpdateTime(0)
	{
        lastSheddingTime = 0;
        loadCnt = 0;
        time = 60000000;
        acctime = time;
        Rtime = time/50000;
        latency = 0;
        cntFullMatch = 1;
	}

	~CepMatch()
	{

	}

	bool init(const char* _defFile, const char* _queryName, const char* _miningPrefix, bool _generateTimeoutEvents, bool _appendTimestamp)
	{
		StreamEvent::setupStdIo();

		if (!m_Definition.loadFile(_defFile))
		{
			fprintf(stderr, "failed to load definition file %s\n", _defFile);
			return false;
		}

		m_Query = !_queryName ? m_Definition.query((size_t)0) : m_Definition.query(_queryName);
		if (!m_Query)
		{
			fprintf(stderr, "query not found");
			return false;
		}

		if(_miningPrefix)
		{
			m_MiningPrefix = _miningPrefix;
			m_Miner.reset(new PredicateMiner(m_Definition, *m_Query));
			_generateTimeoutEvents = true; // important for mining

			for (size_t i = 0; i < m_Query->events.size() - 1; ++i)
			{
				uint32_t eventType = m_Definition.findEventDecl(m_Query->events[i].type.c_str());
				m_Miner->initList(i * 2 + 0, eventType);
				m_Miner->initList(i * 2 + 1, eventType);
			}

			unsigned numCores = thread::hardware_concurrency();
			m_Miner->initWorkerThreads(std::min(numCores - 1, 16u));

			const uint64_t one_min = 60 * 1000 * 1000;
			m_NextMinerUpdateTime = current_utime() + one_min / 6;
		}

		QueryLoader::Callbacks cb;
		cb.insertEvent[PatternMatcher::ST_ACCEPT] = bind(&CepMatch::write_event, this, false, placeholders::_1, placeholders::_2);

		if(_generateTimeoutEvents)
			cb.timeoutEvent = bind(&CepMatch::write_event, this, true, placeholders::_1, placeholders::_2);

		if (!m_Definition.setupPatternMatcher(m_Query, m_Matcher, cb))
		{
			return false;
		}

		m_Query->generateCopyList(m_Query->returnAttr, m_OutEventAttrSrc);

		m_ResultEventType = m_Definition.findEventDecl(m_Query->returnName.c_str());
		m_ResultEventTypeHash = StreamEvent::hash(m_Query->returnName);
		m_ResultAttributeCount = (uint8_t)m_OutEventAttrSrc.size();
		m_GenerateTimeoutEvents = _generateTimeoutEvents;
		m_AppendTimestamp = _appendTimestamp;

		return true;
	}

    bool processBikeEvent()
    {
        StreamEvent event;
        Bike_event RawEvent;
        if(!RawEventQueue.empty())
        {
            RawEvent = RawEventQueue.front();
            RawEventQueue.pop();
        }
        else{
            cout << "event stream queue is empty now!" << endl;
            return false;
        }


        event.attributes[0] = RawEvent.start_time;
        event.attributes[1] = RawEvent.end_time;
        event.attributes[2] = RawEvent.duration_sec;
        event.attributes[3] = RawEvent.start_station_id;
        event.attributes[4] = RawEvent.end_station_id;
        event.attributes[5] = RawEvent.bike_id;
        event.attributes[6] = RawEvent.eventID;

        
        uint64_t t = 86400000000;
        uint64_t ts= 300000000;
        const uint64_t one_min = 60*1000*1000;

//        if(RawEvent.start_time < _day*t)
//            return true;
//
//        //aggeragte the number of partial match into every minute.
//        if(RawEvent.start_time > (_day+1)*t)
//        {
//            return false;
//        } 

        //for(int i=0; i <=6; ++i)
        //    cout << event.attributes[i] << " , "  ;
        //cout << endl;


		event.attributes[Query::DA_ZERO] = 0;
		event.attributes[Query::DA_MAX] = numeric_limits<attr_t>::max();
		event.attributes[Query::DA_CURRENT_TIME] = current_utime();
		//event.attributes[Query::DA_CURRENT_TIME] = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count(); 
		event.attributes[Query::DA_OFFSET] = m_DefAttrOffset;
		event.attributes[Query::DA_ID] = m_DefAttrId;

		const EventDecl* decl = m_Definition.eventDecl(event.typeIndex);
		//assert(event.typeHash == StreamEvent::hash(decl->name));

		m_Matcher.event(event.typeIndex, (attr_t*)event.attributes);

        if(event.attributes[0] > Tick_PM_one_min + one_min) 
        {
            //cout << m_one_min_cnt << " " << NumPartialMatch - Tick_PM_Cnt << endl;
            cout << m_one_min_cnt << " " << NumPartialMatch << endl;

            m_one_min_cnt++;
            Tick_PM_one_min = event.attributes[0];
            Tick_PM_Cnt = NumPartialMatch;
        }

        return true;

    }



    bool readEventStreamFromFiles(string file)
    {
       ifstream ifs;
       ifs.open(file.c_str());
        if( !ifs.is_open())
        {
            cout << "can't open file " << file << endl;
            return false;
        }

        string line;

        while(getline(ifs,line))
        {
            vector<string> dataEvent;
            stringstream lineStream(line);
            string cell;

            while(getline(lineStream,cell,','))
            {
                dataEvent.push_back(cell);
            }

            if(dataEvent[4] == "NULL" || dataEvent[8]=="NULL")
                continue;

            Bike_event RawEvent;
            
            RawEvent.eventID         =  stoll(dataEvent[0]);
            RawEvent.duration_sec    = stoll(dataEvent[1]);
            RawEvent.start_time      = stoll(dataEvent[2]);
            RawEvent.end_time        = stoll(dataEvent[3]);
            RawEvent.start_station_id   = stoll(dataEvent[4]);
            RawEvent.end_station_id     = stoll(dataEvent[8]);
            RawEvent.bike_id           = stoll(dataEvent[12]);
            RawEvent.user_type         = dataEvent[13];
            RawEvent.user_birth         = stoll(dataEvent[14]);
            RawEvent.user_gender         = stoll(dataEvent[15]);


            
            RawEventQueue.push(RawEvent);
            m_Sation_Book[RawEvent.start_station_id] = dataEvent[5];
            m_Sation_Book[RawEvent.end_station_id] = dataEvent[9];

        }

        return true;

    }

    uint64_t RandomInputShedding(double ratio, volatile uint64_t & _eventCnt) 
    {
        uint64_t ShedCnt = 0;

        while(!RawEventQueue.empty())
        {
            ++_eventCnt;
            int dice_roll = m_distribution(m_generator);
            if(dice_roll <=  (ratio *100) )
            {
                RawEventQueue.pop();
                ++ShedCnt;
            }
            else
                processBikeEvent();
        }
        return ShedCnt;
    }

    uint64_t SelectivityInputShedding(double _ratio, volatile uint64_t & _eventCnt)
    {
        int ratio = _ratio*100;
        uint64_t ShedCnt = 0;
        while(!RawEventQueue.empty())
        {
            ++_eventCnt;
            int dice_roll = m_distribution(m_generator);
            if(RawEventQueue.front().user_type == "Customer" && dice_roll <= ratio+10)
            {
                RawEventQueue.pop(); 
                ++ShedCnt;
            }
            else if(RawEventQueue.front().user_type == "Subscriber" && dice_roll <= ratio)
            {
                RawEventQueue.pop(); 
                ++ShedCnt;
            }
            else
            {
                processBikeEvent();
            }
        }
        return ShedCnt;
    }

    uint64_t HybridIrrelevantInputShedding(volatile uint64_t & _eventCnt)
    {
        uint64_t ShedCnt = 0;
        while(!RawEventQueue.empty())
        {
            ++_eventCnt;
            if(eventKeepBooking.count(RawEventQueue.front().end_station_id) == 0)
            {
                RawEventQueue.pop();
                ++ShedCnt;
            }
            else
            {
                processBikeEvent();
            }
        }
        return ShedCnt;
    }




    bool readPMKeepBookings(string file)
    {
       ifstream ifs;
       ifs.open(file.c_str());
        if( !ifs.is_open())
        {
            cout << "can't open file " << file << endl;
            return false;
        }

        string line;

        while(getline(ifs,line))
        {
            vector<string> dataEvent;
            stringstream lineStream(line);
            string cell;

            while(getline(lineStream,cell,','))
            {
                dataEvent.push_back(cell);
            }


            
            int stateID         =  stoi(dataEvent[0]);
            int timeSliceID     =  stoi(dataEvent[1]) - 1;
            int PMkey           =  stoi(dataEvent[2]);

            //cout << "readPMKeepBookings " << m_Matcher.m_States[stateID].PMBooks.size() << endl; 
            //cout << stateID << " " << timeSliceID << " " << PMkey << endl;
            //cout << m_Matcher.m_States[stateID].timeSliceSpan << " " << m_Matcher.m_States[stateID].numTimeSlice << endl;;
            m_Matcher.m_States[stateID].PMBooks[timeSliceID].insert(PMkey);
            eventKeepBooking.insert(PMkey);

        }
    }


	bool processEvent()
	{
		StreamEvent event;

		if (!event.read())
			return false;

		event.attributes[Query::DA_ZERO] = 0;
		event.attributes[Query::DA_MAX] = numeric_limits<attr_t>::max();
		event.attributes[Query::DA_CURRENT_TIME] = current_utime();
		//event.attributes[Query::DA_CURRENT_TIME] = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count(); 
		event.attributes[Query::DA_OFFSET] = m_DefAttrOffset;
		event.attributes[Query::DA_ID] = m_DefAttrId;

		const EventDecl* decl = m_Definition.eventDecl(event.typeIndex);
		assert(event.typeHash == StreamEvent::hash(decl->name));

		m_Matcher.event(event.typeIndex, (attr_t*)event.attributes);

		if (m_Miner)
		{
			m_Miner->flushMatch();
			m_Miner->addEvent(event.typeIndex, (const attr_t*)event.attributes);
			m_Miner->removeTimeouts(event.attributes[0]);
		}

		m_DefAttrId++;
		m_DefAttrOffset += event.offset;
		assert(event.offset > 0);

		if (m_Miner && m_NextMinerUpdateTime <= event.attributes[Query::DA_CURRENT_TIME])
		{
			const uint64_t one_min = 60 * 1000 * 1000;
			m_NextMinerUpdateTime = event.attributes[Query::DA_CURRENT_TIME] + 10 * one_min;
			update_miner();
		}

		return true;
	}

    void printContribution()
    {
        for(auto&& it: m_Matcher.m_States)
        {
            std::cout << endl << "state contributions size == " << it.contributions.size() << endl;
            //for(auto&& iter: it.contributions)
                //cout << iter.first << "appears " << iter.second << "times" << endl;

            std::cout << endl << "state consumptions size == " << it.consumptions.size() << endl;
            //for(auto&& iter: it.consumptions)
            //    cout << iter.first << "appears " << iter.second << "times" << endl;
            std::cout << endl << "state scoreTable size == " << it.scoreTable.size() << endl;
            
        }
    }

    void dumpLatencyBooking(string file)
    {
        ofstream outFile;
        outFile.open(file.c_str());

        for(auto iter : m_Latency_booking)
            outFile << iter.first << "," << iter.second <<  endl;
    }

    

	void update_miner()
	{
		if (!m_Miner)
			return;

		string eqlFilename = m_MiningPrefix + ".eql";
		string scriptFilename = m_MiningPrefix + ".sh";

		ofstream script(scriptFilename, ofstream::out | ofstream::trunc);

		QueryLoader dst = m_Definition;
		for (size_t i = 0; i < m_Query->events.size() - 1; ++i)
		{
			m_Miner->printResult(i * 2, i * 2 + 1);

			auto options = m_Miner->generateResult(i * 2, i * 2 + 1);
			for (auto& it : options)
			{
				Query q = m_Miner->buildPredicateQuery(dst, *m_Query, (uint32_t)i, it);

				ostringstream namestr;
				namestr << q.name << "_mined_" << i << '_' << it.eventId;
				for (size_t i = 0; i < it.numAttr; ++i)
					namestr << '_' << it.attrIdx[i];

				q.name = namestr.str();

				script << "$CEP_CMD -c " << eqlFilename << " -q " << q.name << " < $CEP_IN > ${CEP_OUT}" << q.name << " &" << endl;
				dst.addQuery(move(q));
			}
		}

		dst.storeFile(eqlFilename.c_str());
	}

//protected:

	void write_event(bool _timeout, uint32_t _state, const attr_t* _attributes)
	{
		StreamEvent r;
		r.typeIndex = m_ResultEventType;
		r.typeHash = m_ResultEventTypeHash;
		r.attributeCount = m_ResultAttributeCount;
		r.flags = 0;

		if (_timeout)
		{
			r.flags = StreamEvent::F_TIMEOUT;
			r.timeoutState = (uint8_t)(_state - 1);
		}

		if (m_AppendTimestamp)
		{
			r.flags |= StreamEvent::F_TIMESTAMP;
			r.attributes[r.attributeCount++] = current_utime();
		}

		uint64_t* outattr_it = r.attributes;
        string hotPath;
   //     cout << "0," << flush;
		for (auto it : m_OutEventAttrSrc)
        {
			*outattr_it++ = _attributes[it];
    //        cout << _attributes[it] << "," << flush;
            //hotPath += to_string( _attributes[it]) + ",";
            //hotPath += m_Sation_Book[ _attributes[it]] + "---";
        }
     //   cout << endl;
        //++m_HotPaths[hotPath];


        //cout << "latency  in write event" << _attributes[Query::DA_FULL_MATCH_TIME] << "--" <<   _attributes[Query::DA_CURRENT_TIME] << "--" << r.attributes[Query::DA_CURRENT_TIME] << endl; 

		//r.write();

        //monitoring load shedding: if latency exceeds threshold for 2 times, call load shedding
        //uint64_t la = _attributes[Query::DA_FULL_MATCH_TIME] - _attributes[Query::DA_CURRENT_TIME]; 
        //if(_attributes[Query::DA_FULL_MATCH_TIME] / 50000 == time / 50000)
        //{
        //    acctime += _attributes[Query::DA_FULL_MATCH_TIME];
        //    latency += la;
        //    Rtime = time/50000; 
        //    cntFullMatch++;
        //}
        //else
        //{
        //    if(latency/cntFullMatch > 150 && _attributes[Query::DA_FULL_MATCH_TIME] - lastSheddingTime > 3000000)
        //    {
        //        //m_Matcher.randomLoadShedding();
        //        //m_Matcher.loadShedding();
        //        lastSheddingTime = _attributes[Query::DA_FULL_MATCH_TIME];
        //        ++loadCnt;
        //    }
        //    cntFullMatch = 1;
        //    time = _attributes[Query::DA_FULL_MATCH_TIME];
        //    latency = la;
        //    acctime = time;
        //    Rtime = time/50000;
        //    
        //}

        uint64_t lateny = _attributes[Query::DA_FULL_MATCH_TIME] - _attributes[Query::DA_CURRENT_TIME];
         m_Latency_booking[lateny]++;
        ACCLantency += lateny;
        
        
        
        if(_attributes[Query::DA_FULL_MATCH_TIME] - _attributes[Query::DA_CURRENT_TIME] > 150 && _attributes[Query::DA_FULL_MATCH_TIME] - lastSheddingTime > 3000000)
        {
            
            if(m_Matcher.latencyFlag == true)
            {
                //lastSheddingTime = _attributes[Query::DA_FULL_MATCH_TIME]; 
               // m_Matcher.loadShedding();
                //m_Matcher.randomLoadShedding();
                lastSheddingTime = _attributes[Query::DA_FULL_MATCH_TIME]; 
                ++loadCnt;
                
            }

            else
                m_Matcher.latencyFlag = true;
        }
        else
        {
            m_Matcher.latencyFlag = false;
        }


        //////


		if (m_Miner)
		{
			if (_timeout)
			{
				size_t idx = (_state - 1) * 2 + 1;
				uint32_t eventA = (uint32_t)r.attributes[_state];
				uint32_t eventB = (uint32_t)m_DefAttrId;

				m_Miner->addMatch(eventA, eventB, idx);
			}
			else
			{
				for (uint32_t s = 1; s < _state; ++s)
				{
					size_t idx = (s - 1) * 2;
					uint32_t eventA = (uint32_t)r.attributes[s];
					uint32_t eventB = (uint32_t)r.attributes[s + 1];

					m_Miner->addMatch(eventA, eventB + 1, idx);
				}
			}
		}
	}

//private:
	QueryLoader			m_Definition;
	const Query*		m_Query;

	unique_ptr<PredicateMiner> m_Miner;
	string				m_MiningPrefix;
	PatternMatcher		m_Matcher;
	vector<uint32_t>	m_OutEventAttrSrc;

	uint16_t			m_ResultEventType;
	uint32_t			m_ResultEventTypeHash;
	uint8_t				m_ResultAttributeCount;
	bool				m_GenerateTimeoutEvents;
	bool				m_AppendTimestamp;

	uint64_t			m_DefAttrId;
	uint64_t			m_DefAttrOffset;

	uint64_t			m_NextMinerUpdateTime;
    uint64_t            lastSheddingTime;

    uint64_t            time;
    uint64_t            latency;
    uint64_t            acctime;
    uint64_t            Rtime;
    uint64_t            cntFullMatch;

    uint64_t            Tick_PM_Cnt = 0;
    uint64_t            Tick_PM_one_min = 0;
    uint64_t            m_one_min_cnt = 1;

    queue<Bike_event>   RawEventQueue;

    uint64_t            lastEventTime = 0;

    uint64_t cnt = 0;

    int loadCnt;
    map<string, attr_t> m_HotPaths;
    map<attr_t, string> m_Sation_Book;
    map<int,uint64_t>  m_Latency_booking;
    unordered_set<attr_t> eventKeepBooking;
};

//bool PatternMatcher::loadMonitoringFlag = false;
int main(int _argc, char* _argv[])
{
	init_utime();

	const char* deffile = "default.eql";
	const char* queryName = 0;
	const char* monitorFile = 0;
	const char* miningPrefix = 0;
	bool captureTimeouts = false;
	bool appendTimestamp = false;

    bool SheddingIrrelevantFlag = false;
    bool RPMSFlag = false;
    bool SlPMSFlag = false;
    bool RISFlag = false;
    bool SlISFlag = false;

    double RPMSRatio = 0;
    double SlPMSRatio = 0;
    double RISRatio = 0;
    double SlISRatio = 0;
            
            

    int timeSlice = 1;
    //string streamFile = "/home/bo/CEP_load_shedding/data/bike_sharing/bike_sharing_cleaned.csv";
    string streamFile = "/home/bo/CEP_load_shedding/data/bike_sharing/newTmp.csv";
    //string streamFile = "/home/bo/CEP_load_shedding/data/bike_sharing/6.csv";
    string PMKeepingkeysFile = "/home/bo/CEP_load_shedding/data/bike_sharing/PM_keys_3TTL_1h_g10.csv";

	int c;
	while ((c = getopt(_argc, _argv, "c:q:p:m:T:w:x:y:z:tsI")) != -1)
	{
		switch (c)
		{
		case 'c':
			deffile = optarg;
			break;
		case 'q':
			queryName = optarg;
			break;
		case 'p':
			monitorFile = optarg;
			break;
		case 'm':
			miningPrefix = optarg;
			break;
		case 't':
			captureTimeouts = true;
			break;
		case 's':
			appendTimestamp = true;
			break;
        case 'T':
            timeSlice = stoi(string(optarg));
            break;
        case 'I':
            SheddingIrrelevantFlag = true;
            break;
        case 'w':
            RPMSFlag = true;
            RPMSRatio = stod(string(optarg));
            break;
        case 'x':
            SlPMSFlag = true;
            SlPMSRatio = stod(string(optarg));
            break;
        case 'y':
            RISFlag = true; 
            RISRatio = stod(string(optarg));
            break;
        case 'z':
            SlISFlag = true;
            SlISRatio = stod(string(optarg));
            break;
            
		default:
			abort();
		}
	}

	CepMatch prog;
	if (!prog.init(deffile, queryName, miningPrefix, captureTimeouts, appendTimestamp))
		return 1;

    //prog.m_Matcher.print();
	volatile uint64_t eventCounter = 0;



    //PatternMatcher::setMonitoringLoadOn();
    PatternMatcher::setMonitoringLoadOff();


    attr_t _timeWindow = 1;
    _timeWindow *=60;
    _timeWindow *=60;
    _timeWindow *=1000000;

    attr_t _tsSpan = _timeWindow/timeSlice + 1;
    
    prog.m_Matcher.setTimeWindow();
    prog.m_Matcher.setTimeSliceNum(timeSlice);
    //cout << "[main] flag 1 " << endl;
    if(SheddingIrrelevantFlag)
    {
        prog.readPMKeepBookings(PMKeepingkeysFile);
        prog.m_Matcher.m_States[1].PMBooks[0].erase(448);
    }

    prog.readEventStreamFromFiles(streamFile);

	MonitorThread monitor;
	monitor.addValue(&current_utime);
	monitor.addValue(&eventCounter, true);
    monitor.addValue(&NumPartialMatch,false);

	if (monitorFile)
		monitor.start(monitorFile);
    
    //setting up the indexes
    prog.m_Matcher.m_States[1].timePointIdx = 0;
    prog.m_Matcher.m_States[1].PMKeyIdx = 2;

    prog.m_Matcher.m_States[2].timePointIdx = 4;
    prog.m_Matcher.m_States[2].PMKeyIdx = 5;

    prog.m_Matcher.m_States[3].timePointIdx = 6;
    prog.m_Matcher.m_States[3].PMKeyIdx = 7;

    prog.m_Matcher.m_States[4].timePointIdx = 8;
    prog.m_Matcher.m_States[4].PMKeyIdx = 9;

    for(auto &s: prog.m_Matcher.m_States)
    {
        s.sheddingIrrelaventFlag = SheddingIrrelevantFlag;
        s.timeSliceSpan = _tsSpan;
    }
    


   // cout << "monitoring " << PatternMatcher::MonitoringLoad() << endl;
    //int loadCnt = 0;
    
	//while(prog.processEvent())
//    for(int t = 0; t <= 30; ++t)
//    {
//        cout << "bike trips state in day " << t+1 << endl;
//        cout << "========================" << endl;

   // cout << "[main] flag 2 " << endl;

    if(RPMSFlag)
        prog.m_Matcher.setRandomPMShedding(RPMSRatio);

    if(SlPMSFlag)
        prog.m_Matcher.setSelectivityPMShedding(SlPMSRatio);


   uint64_t sheddingEventCnt = 0;

        while(prog.processBikeEvent())
        {
            eventCounter++;
           if(RISFlag)
               sheddingEventCnt = prog.RandomInputShedding(RISRatio, eventCounter);

           if(SlISFlag)
                sheddingEventCnt = prog.SelectivityInputShedding(SlISRatio, eventCounter);

            if(SheddingIrrelevantFlag)
                sheddingEventCnt = prog.HybridIrrelevantInputShedding(eventCounter);
            //if(false && PatternMatcher::MonitoringLoad() == true){
            //    if(eventCounter  == 1000000 )
            //    {
            //        PatternMatcher::setMonitoringLoadOff();
            //        cout << "call loadshedding" << endl;

            //        time_point<high_resolution_clock> t0 = high_resolution_clock::now();

            //        prog.m_Matcher.computeScores4LoadShedding();
            //        //prog.m_Matcher.loadShedding();

            //        time_point<high_resolution_clock> t1 = high_resolution_clock::now();
            //        cout << "1st load shedding time " << duration_cast<microseconds>(t1 - t0).count() << endl;
            //        //prog.loadCnt++;

            //    }

                //if(eventCounter > 100000 && eventCounter % 100000 == 1)
                //{

                //    time_point<high_resolution_clock> t0 = high_resolution_clock::now();
                //    prog.m_Matcher.loadShedding();
                //    time_point<high_resolution_clock> t1 = high_resolution_clock::now();
                //    cout << "laod shedding time " << duration_cast<microseconds>(t1 - t0).count() << endl;
                //    ++loadCnt;
                //}
            //}
        }





        //prog.update_miner();
        //prog.printContribution();
        //std::cout << "perform " << prog.loadCnt << "loadshedding" << endl;
//        std::cout << "eventCn " << eventCounter << endl;
//        std::cout << "#full Match " << NumFullMatch << endl;
//        std::cout << "#high latecny (> 150)" << NumHighLatency<< endl;
//        std::cout << "#NumPartialMatch " << NumPartialMatch << endl;
//        std::cout << "#NumDropedPM " << NumSheddingPM << endl;
//        std::cout << "#NumDropedInputEvents " << sheddingEventCnt << endl;
//        if(NumFullMatch)
//            cout << "avg latency: " << (double) (ACCLantency/NumFullMatch) << endl;

        prog.dumpLatencyBooking("latency.csv");

        multimap<attr_t, string> SortedHotPaths;
        for(auto &a: prog.m_HotPaths)
            if(a.second > 10)
            {
                SortedHotPaths.insert( {a.second, a.first});	
            }
        for(auto &a: SortedHotPaths)
            cout << a.first << " : " << a.second << endl;

        prog.m_HotPaths.clear();
//    }
    return 0;
}
