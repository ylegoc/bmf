const database = require('../services/database');

module.exports.add = (mentor, callback) => {

    console.log('adding ' + mentor.pseudo + ' @ ' + mentor.startLat + ', ' + mentor.startLng + ' - ' + mentor.endLat + ', ' + mentor.endLng);

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