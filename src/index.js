var yaml = require('js-yaml');
var iracing = require('../build/Release/iracing.node')();
var tickInterval = null
var dataInterval = null;
var TIMEOUT = 1000;
var DATATIMEOUT = 0;
var initialized = false;

var driverInfo = {};

var tick = function() {
	if(iracing.waitForDataReady(60)) {
		sessionData = yaml.load(iracing.getSessionYAML());
		if(!initialized) {
			initialized = true;
			driverInfo = parseDrivers(sessionData.DriverInfo);
			dataInterval = setInterval(dataTick, DATATIMEOUT);
		};
	}
	else {
		initialized = false;
		if(dataInterval) { 
			clearInterval(dataInterval);
			dataInterval = null;
		}
	}
}

var dataTick = function() {
	if(!initialized) return;
	
	var carIdx = iracing.getTelemetry("CamCarIdx");
}

var parseDrivers = function(dInfo) {
	dInfo.DriversByIndex = {};
	for(var i = 0; i < dInfo.Drivers.length; i++) {
		dInfo.DriversByIndex[dInfo.Drivers[i].CarIdx] = dInfo.Drivers[i];
	}
	return dInfo;
}

tick();
tickInterval = setInterval(tick, TIMEOUT);