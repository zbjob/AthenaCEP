
/*
This inline function was automatically generated using DecisionTreeToCpp Converter
It takes feature vector as single argument:
feature_vector[0] - Av1
feature_vector[1] - Bv2

It returns index of predicted class:
0 - 1
1 - 2
2 - 3
3 - 4

Simply include this file to your project and use it
*/
#include <vector>
#include <iostream>
#include <ctime>

inline int ST2_decision_tree(const std::vector<double> & feature_vector) 
{
	if (feature_vector.at(0) <= 54.5) {
		if (feature_vector.at(0) <= 37.5) {
			if (feature_vector.at(0) <= 36.5) {
				return 0;
			}
			else {
				if (feature_vector.at(1) <= 54.5) {
					return 0;
				}
				else {
					if (feature_vector.at(1) <= 55.5) {
						return 2;
					}
					else {
						return 0;
					}
				}
			}
		}
		else {
			if (feature_vector.at(1) <= 48.5) {
				return 0;
			}
			else {
				if (feature_vector.at(1) <= 59.5) {
					if (feature_vector.at(0) <= 51.5) {
						if (feature_vector.at(0) <= 40.5) {
							return 2;
						}
						else {
							return 1;
						}
					}
					else {
						if (feature_vector.at(1) <= 50.5) {
							return 0;
						}
						else {
							return 2;
						}
					}
				}
				else {
					if (feature_vector.at(1) <= 60.5) {
						if (feature_vector.at(0) <= 42.5) {
							return 0;
						}
						else {
							return 2;
						}
					}
					else {
						return 0;
					}
				}
			}
		}
	}
	else {
		if (feature_vector.at(0) <= 55.5) {
			if (feature_vector.at(1) <= 53.5) {
				return 0;
			}
			else {
				if (feature_vector.at(1) <= 55.5) {
					return 2;
				}
				else {
					return 0;
				}
			}
		}
		else {
			return 0;
		}
	}
}

inline int OptST2_DST(int x0, int x1)
{
    if( x0==37 && x1==55 || 
        x0>37 && x0 <= 40 && x1>48 && x1 <=59  ||
        x0>51 && x0 <= 54 && x1>50 && x1 <= 59 ||
        x0>43 && x0 <=54 && x1>59 && x1 <=60 ||
        x0==55 && x1>53 && x1 <=55)
        return 2;
    else if(x0>40 && x0 <= 51 && x1 >48 && x1 <=59)
        return 1;
    else
        return 0;
}





















