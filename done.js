$(document).ready(function () {
    console.log("done.js loaded");

    // Extract query parameters from URL
    const urlParams = new URLSearchParams(window.location.search);
    const game_no = parseInt(urlParams.get('game_no'), 10); // Make sure this retrieves a valid value
    const turn_no = parseInt(urlParams.get('turn_no'), 10);

    console.log("Game Number in /done.js:", game_no);
    console.log("Turn Number in /done.js:", turn_no);

    if (!game_no || isNaN(turn_no)) {
        console.error("Missing or invalid game_no or turn_no in the URL.");
        return;
    }

    function checkForNewTurn() {
        console.log("Polling server for new turn...");

        // Use GET request now (Tried Post didn't work)
        $.get(
            "/check_new_turn",
            { game_no, turn_no }, // Pass parameters in the query string
            function (data) {
                console.log("Server response:", data);

                // Check if new turn and current one are different
                if (data.new_turn_no && data.new_turn_no !== data.turn_no) {
                    console.log("New turn detected. Redirecting to player.html...");
                    const redirectUrl = `/player.html?game_no=${data.game_no}&turn_no=${data.new_turn_no}&board=${encodeURIComponent(data.board)}`;
                    window.location.href = redirectUrl;
                } else {
                    console.log("No valid new turn detected. Staying on current page.");
                }
            },
            "json"
        ).fail(function (error) {
            console.error("Error polling server:", error);
        });
    }

    // Poll server every second
    setInterval(checkForNewTurn, 1000);
});
