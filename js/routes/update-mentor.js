const express = require('express');
const router = express.Router();
const mentorController = require('../controllers/mentor-controller');

router.post('/', (req, res, next) => {

    let startArray = req.body.start.split(",");
    let startLat = parseFloat(startArray[0]);
    let startLng = parseFloat(startArray[1]);

    let endArray = req.body.end.split(",");
    let endLat = parseFloat(endArray[0]);
    let endLng = parseFloat(endArray[1]);

    // TODO: error to manage.

    mentorController.update({
        "pseudo": req.body.pseudo,
        "mail": req.body.mail,
        "startLat": startLat,
        "startLng": startLng,
        "endLat": endLat,
        "endLng": endLng
    },
    (result) => {
        res.render('mentor-update-success', {
            "pseudo": result.pseudo,
            "distance": result.distance
        })
    });

    
});

module.exports = router;