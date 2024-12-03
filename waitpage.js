$(document).ready(function () {
    console.log("waitpage.js loaded"); // Debugging log
    console.log("Game Number:", gameNo); // Ensure gameNo is set correctly

    function checkForOpponent() {
        console.log("Polling server for opponent..."); // Debugging log
        $.post('/check_black_handle', { game_no: gameNo }, function (data) {
            console.log("Response from server:", data); // Log server response
            if (data.black_handle) {
                window.location.href = `/player.html?game_no=${gameNo}&turn_no=1`;
            }
        }, "json");
    }

    // Poll the server every second
    setInterval(checkForOpponent, 1000);
});
