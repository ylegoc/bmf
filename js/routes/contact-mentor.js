const express = require('express');
const router = express.Router();

router.post('/', (req, res, next) => {

    res.render('contact-mentor', {
        "title": "Contacter un Mentor",
        "mentorId": req.body.mentorId,
        "mentorPseudo": req.body.mentorPseudo
    });
    
});

module.exports = router;