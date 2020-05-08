const express = require('express');
const router = express.Router();
const mentorController = require('../controllers/mentor-controller');

router.post('/', (req, res, next) => {

    // Get missing data from mentorController.
    mentorController.find(req.body.mail, (mentor) => {

        // TODO: error page if not found.

        res.render('edit-mentor', {
            "title": "Editer un Mentor",
            "pseudo": mentor.pseudo,
            "mail": mentor.mail,
            // We must revert lat and lng.
            "startLat": mentor.start[1],
            "startLng": mentor.start[0],
            "endLat": mentor.end[1],
            "endLng": mentor.end[0]
        });
    })

    
});

module.exports = router;