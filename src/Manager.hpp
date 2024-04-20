#pragma once

class Manager {

protected:
	static Manager* instance;
public:
	float originalVolume = 0.0f;
	bool isWherwin = false;

	static Manager* getSharedInstance() {
		if (!instance) {
			instance = new Manager();
		}
		return instance;
	}

};