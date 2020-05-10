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
const findMentor = require('./routes/find-mentor');
const displayMentors = require('./routes/display-mentors');
const refindMentor = require('./routes/refind-mentor');
const contactMentor = require('./routes/contact-mentor');
const mailMentor = require('./routes/mail-mentor');

const app = express();
const port = 5000;

// Get the args.
let args = process.argv.slice(2);

if (args.length < 2) {
    console.log('Arguments: url db');
    process.exit(1);
}

// Database.
const url = args[0];
const db = args[1];

database.connect(url, db, () => {
    console.log('Database connected');
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
app.use('/find_mentor', findMentor);
app.use('/display_mentors', displayMentors);
app.use('/refind_mentor', refindMentor);
app.use('/contact_mentor', contactMentor);
app.use('/mail_mentor', mailMentor);

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