const database = require('../services/database');
const bmf = require('../services/bmf');

module.exports.add = (mentor, callback) => {

    // Calculate distance.
    let distance = bmf.getDistance(mentor.startLng, mentor.startLat, mentor.endLng, mentor.endLat);

    let mentorDocument = {
        "pseudo": mentor.pseudo,
        "mail": mentor.mail,
        "start": [ mentor.startLng, mentor.startLat ],
        "end": [ mentor.endLng, mentor.endLat ],
        "dist": distance
    }

    database.addMentor(mentorDocument, callback);
}

module.exports.find = (mail, callback) => {

    let mentorFilter = {
        "mail": mail
    }

    database.findMentor(mentorFilter, callback);
}

module.exports.update = (mentor, callback) => {

    // Calculate distance.
    let distance = bmf.getDistance(mentor.startLng, mentor.startLat, mentor.endLng, mentor.endLat);

    let mentorFilter = {
        "mail": mentor.mail
    }

    let mentorDocument = {
        "pseudo": mentor.pseudo,
        "mail": mentor.mail,
        "start": [ mentor.startLng, mentor.startLat ],
        "end": [ mentor.endLng, mentor.endLat ],
        "dist": distance
    }

    database.updateMentor(mentorFilter, mentorDocument, callback);
}

module.exports.delete = (mail, callback) => {

    let mentorFilter = {
        "mail": mail
    }

    database.deleteMentor(mentorFilter, callback);
}

module.exports.findBest = (startLng, startLat, endLng, endLat, searchType, numberOfResults, callback) => {

    let result = bmf.findBestMentors(startLng, startLat, endLng, endLat, searchType, numberOfResults);

    callback(result);
}