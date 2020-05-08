const express = require('express');
const router = express.Router();
const mentorController = require('../controllers/mentor-controller');

router.post('/', (req, res, next) => {

    // Get mentor from from mentorController.
    mentorController.findById(req.body.mentorId, (mentor) => {

        // TODO: error page if not found.

        mentorController.sendMailToMentor(mentor.mail, req.body.message, () => {

            res.render('mentor-mail-success', {
                "pseudo": mentor.pseudo
            });
        });
    })

    
    
});

module.exports = router;