const express = require('express');
const router = express.Router();
const datafile = require('../js/datafile');

// Pass the datafile to the info page.
router.get('/', (req, res, next) => {
  res.render('info', datafile);
});

module.exports = router;