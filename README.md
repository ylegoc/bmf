# BMF
Bike Mentor Finder

# Install MongoDB 5

On Ubuntu, follow https://askubuntu.com/questions/842592/apt-get-fails-on-16-04-or-18-04-installing-mongodb/842599#842599

Create a user:

```
$ mongo
```

```
use admin
db.createUser(
    {
      user: "mongo",
      pwd: "mongo",
      roles: [ { role: "userAdminAnyDatabase", db: "admin" }, "readWriteAnyDatabase" ]
      }
    )
```

# Compile and install the C++ sources

```
$ cd build-cpp
$ cmake ..
$ sudo cmake --build . --target install
```

# Compile and install the web site

A node.js version is required (>= 16)

Set the npm proxy if necessary:

```
$ npm config set proxy <url>
$ npm config set https-proxy <url>
```

Install node-gyp if necessary:

```
$ sudo -E npm install -g node-gyp
```

```
$ npm install
$ ./node-gyp-rebuild.sh
```

# Init the database with random mentors

```
$ cd build-cpp
$ ./bmfinittest 100
```

Will initialize with 100 mentors.

# Run the web site

Ensure that *libbmf.so* is accessible:
```
$ export LD_LIBRARY_PATH=/usr/local/lib
```

Run the server:
```
$ node ./js/app.js mongodb://mongo:mongo@localhost:27017 bmf-test
```

Open a browser and open the url http://localhost:5000/
