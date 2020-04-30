
module.exports.add = (mentor) => {

    console.log('adding ' + mentor.pseudo + ' @ ' + mentor.startLat + ', ' + mentor.startLng + ' - ' + mentor.endLat + ', ' + mentor.endLng);
    return {
        "ok": true
    }
}