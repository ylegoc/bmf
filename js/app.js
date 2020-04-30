const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');

const database = require('./services/database');
const createMentor = require('./routes/create-mentor');
const addMentor = require('./routes/add-mentor');

const app = express();
const port = 3000;

// Database.
const url = "mongodb://mongo:mongo@localhost:27017";
const db = "bmf-proto";

database.connect(url, db, () => {
    database.test();
});

// Resources.
app.use(express.static('../public'));

// Bodyparser.
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

// Routes.
app.use('/create_mentor', createMentor);
app.use('/add_mentor', addMentor);

// Set the view engine.
app.set('view engine', 'ejs');

// Default route.
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, '../public', 'index.html'));
});

// Listen to the port defined previously.
app.listen(port, () => {
    console.log('BMF web app');
});