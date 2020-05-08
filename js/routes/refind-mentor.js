const express = require('express');
const router = express.Router();

router.post('/', (req, res, next) => {
    res.render('find-mentor', {
        "title": "Trouver un Mentor",
        "startLat": req.body.startLat,
        "startLng": req.body.startLng,
        "endLat": req.body.endLat,
        "endLng": req.body.endLng
    });
});

module.exports = router;