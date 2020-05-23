const database = require('../services/database');
const bmf = require('../services/bmf');
const mongo = require('mongodb');
const ObjectId = mongo.ObjectId;

module.exports.add = (mentor, callback) => {

    // Calculate distance.
    let distance = bmf.getDistance(mentor.startLng, mentor.startLat, mentor.endLng, mentor.endLat);

    let mentorObject = {
        "pseudo": mentor.pseudo,
        "mail": mentor.mail,
        "start": [ mentor.startLng, mentor.startLat ],
        "end": [ mentor.endLng, mentor.endLat ],
        "dist": distance
    }

    database.addMentor(mentorObject, callback);
}

module.exports.find = (mail, callback) => {

    let mentorFilter = {
        "mail": mail
    }

    database.findMentor(mentorFilter, callback);
}

module.exports.findById = (id, callback) => {

    let mentorFilter = {
        "_id": new ObjectId(id)
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

module.exports.sendMailToMentor = (mail, message, callback) => {

    console.log('sending mail to ' + mail + " : " + message);

    callback();
}

