#pragma once

using namespace geode::prelude;

class Manager {

protected:
	static Manager* instance;
public:

	float orignalVolume = 0.0f;
	
	static Manager* getSharedInstance(){

		if (!instance) {
			instance = new Manager();
		};
		return instance;
	}

};