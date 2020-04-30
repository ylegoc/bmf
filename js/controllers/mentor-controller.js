const database = require('../services/database');

module.exports.add = (mentor, callback) => {

    // TODO: calculate dist.

    let mentorDocument = {
        "pseudo": mentor.pseudo,
        "mail": mentor.mail,
        "start": [ mentor.startLng, mentor.startLat ],
        "end": [ mentor.endLng, mentor.endLat ],
        "dist": 10
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

    // TODO: calculate dist.

    let mentorFilter = {
        "mail": mentor.mail
    }

    let mentorDocument = {
        "pseudo": mentor.pseudo,
        "mail": mentor.mail,
        "start": [ mentor.startLng, mentor.startLat ],
        "end": [ mentor.endLng, mentor.endLat ],
        "dist": 10
    }

    database.updateMentor(mentorFilter, mentorDocument, callback);
}

module.exports.delete = (mail, callback) => {

    let mentorFilter = {
        "mail": mail
    }

    database.deleteMentor(mentorFilter, callback);
}