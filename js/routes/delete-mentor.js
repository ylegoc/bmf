const express = require('express');
const router = express.Router();
const mentorController = require('../controllers/mentor-controller');

router.post('/', (req, res, next) => {

    mentorController.delete(req.body.mail,
    (result) => {
        res.render('mentor-deletion-success', {
        })
    });
    
});

module.exports = router;