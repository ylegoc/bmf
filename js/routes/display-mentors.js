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

    // Find best mentors from mentorController.
    mentorController.findBest(startLng, startLat, endLng, endLat, "TRAJECTORY", 10, (result) => {

        // TODO: error page if not found.

        res.render('display-mentors', {
            "title": "Liste de Mentors",
            "result": result
        });
    })

    
});

module.exports = router;