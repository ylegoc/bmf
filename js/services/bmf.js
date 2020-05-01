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

        let result = [];

        let mentors = bmfAPI.findBestMentors(startLng, startLat, endLng, endLat, searchType, numberOfResults);
        let length = mentors.length;
        for (let i = 0; i < length; i++) {
            // The mentor property is a string that we must parse.
            let mentorResult = JSON.parse(mentors[i].mentor);

            // Add the score.
            mentorResult.score = mentors[i].score;

            // Push the result.
            result.push(mentorResult);
        }

        return result;
    }
}

module.exports = new BMF();