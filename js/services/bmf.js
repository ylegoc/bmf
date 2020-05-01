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

    findBestMentors(startLng, startLat, endLng, endLat, searchType, numberOfResults) {
        return bmfAPI.findBestMentors(startLng, startLat, endLng, endLat, searchType, numberOfResults);
    }
}

module.exports = new BMF();