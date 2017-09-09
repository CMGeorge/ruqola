function stringToColour(str) {
    var hash = 0;
    for (var i = 0; i < str.length; i++) {
        hash = str.charCodeAt(i) + ((hash << 5) - hash);
    }
    var colour = '#';
    for (var j = 0; j < 3; j++) {
        var value = (hash >> (j * 8)) & 0xFF;
        colour += ('00' + value.toString(16)).substr(-2);
    }
    return colour;
}

function displayDateTime(timestamp)
{
    return Qt.formatDate(new Date(timestamp), i18n("yyyy-MM-dd")) + "\n" + Qt.formatTime(new Date(timestamp), i18n("hh:mm"));
}
