bmfAPI = require('../../build/Release/addonbmf');

class BMF {

    constructor() {
    }

    init(url, db) {
        bmfAPI.init(url, db);
    }

    getDistance(startLng, startLat, endLng, endLat) {
        return bmfAPI.getDistance(startLng, startLat, endLng, endLat);
    }
}

module.exports = new BMF();