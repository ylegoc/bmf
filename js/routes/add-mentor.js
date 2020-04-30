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

  // Error to manage.

  let result = mentorController.add({
    "pseudo": req.body.pseudo,
    "mail": req.body.mail,
    "startLat": startLat,
    "startLng": startLng,
    "endLat": endLat,
    "endLng": endLng
  });

  if (result.ok) {
    res.send('Ajouté ' + req.body.pseudo);
  }
});

module.exports = router;