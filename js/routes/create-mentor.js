const express = require('express');
const router = express.Router();

router.get('/', (req, res, next) => {
    res.render('create-mentor', {
        "title": "Créer un Mentor",
        "pseudo": "",
        "mail": "",
        "start": "",
        "end": ""
    });
});

module.exports = router;