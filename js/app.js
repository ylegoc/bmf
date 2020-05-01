const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');

const database = require('./services/database');
const bmf = require('./services/bmf');

const createMentor = require('./routes/create-mentor');
const addMentor = require('./routes/add-mentor');
const connectMentor = require('./routes/connect-mentor');
const editMentor = require('./routes/edit-mentor');
const updateMentor = require('./routes/update-mentor');
const deleteMentor = require('./routes/delete-mentor');

const app = express();
const port = 3000;

// Database.
const url = "mongodb://mongo:mongo@localhost:27017";
const db = "bmf-proto";

database.connect(url, db, () => {
    database.test();
});

bmf.init(url, db);

// Resources.
app.use(express.static('../public'));

// Bodyparser.
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

// Routes.
app.use('/create_mentor', createMentor);
app.use('/add_mentor', addMentor);
app.use('/connect_mentor', connectMentor);
app.use('/edit_mentor', editMentor);
app.use('/update_mentor', updateMentor);
app.use('/delete_mentor', deleteMentor);

// Set the view engine.
app.set('view engine', 'ejs');

// Default routes.
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, '../public', 'index.html'));
});

app.get('/*', (req, res) => {
    res.sendFile(path.join(__dirname, '../public', 'index.html'));
})

// Listen to the port defined previously.
app.listen(port, () => {
    console.log('BMF web app');
});