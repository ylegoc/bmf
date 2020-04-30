const https = require('https');

module.exports.getPoint = (address) => {
    
    const options = {
        hostname: 'nominatim.openstreetmap.org',
        port: 443,
        path: '/search?q=3+louis+le+cardonnel,+grenoble,+france&format=geocodejson',
        method: 'GET'
      }
      
      const req = https.request(options, res => {
        console.log(`statusCode: ${res.statusCode}`)
      
        res.on('data', d => {
          process.stdout.write(d)
        })
      })
      
      req.on('error', error => {
        console.error(error)
      })
      
      req.end();

    // https://nominatim.openstreetmap.org/search?q=3+louis+le+cardonnel,+grenoble,+france&format=geocodejson
}