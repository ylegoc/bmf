const express = require('express');
const router = express.Router();

router.get('/', (req, res, next) => {
    res.render('find-mentor', {
        "title": "Trouver un Mentor"
    });
});

module.exports = router;