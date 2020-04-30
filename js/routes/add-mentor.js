const express = require('express');
const router = express.Router();
const mentorController = require('../controllers/mentor-controller');

router.post('/', (req, res, next) => {

    let startArray = req.body.start.split(",");
    let startLat = startArray[0];
    let startLng = startArray[1];

    let endArray = req.body.end.split(",");
    let endLat = endArray[0];
    let endLng = endArray[1];

    // TODO: error to manage.

    mentorController.add({
        "pseudo": req.body.pseudo,
        "mail": req.body.mail,
        "startLat": startLat,
        "startLng": startLng,
        "endLat": endLat,
        "endLng": endLng
    },
    (result) => {
        //res.send('Ajouté ' + req.body.pseudo);
        res.render('mentor-success', {
            "pseudo": result.pseudo,
            "distance": result.distance
        })
    });

    
});

module.exports = router;