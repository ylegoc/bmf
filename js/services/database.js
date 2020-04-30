const mongoClient = require('mongodb').MongoClient;

class Database {

    constructor() {
        this._db = null;
        this._dbo = null;
    }

    connect(url, database, callback) {

        mongoClient.connect(url, (err, db) => {
            if (err) {
                throw err;
            }
            this._db = db;
            this._dbo = db.db(database);

            callback();
        });

    }

    test() {
        this._dbo.collection("mentors").find().toArray((err, result) => {
            if (err) {
                throw err;
            }
            console.log(result);
        });
    }

    close() {
        this._db.close();
    }

    addMentor(mentor, callback) {

        this._dbo.collection("mentors").insertOne(mentor, (err, res) => {
            if (err) {
                throw err;
            }
            console.log("1 mentor inserted");

            let result = {
                "pseudo": mentor.pseudo,
                "distance": mentor.dist
            }

            callback(result);
        });
    }

}

module.exports = new Database();