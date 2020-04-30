const express = require('express');
const router = express.Router();

router.get('/', (req, res, next) => {
    res.render('connect-mentor', {
        "title": "Se connecter"
    });
});

module.exports = router;