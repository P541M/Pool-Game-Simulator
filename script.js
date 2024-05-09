$(document).ready(function() {
    const cueBall = $('#cue-ball');
    let isDragging = false;
    let currentLine = null;

    updatePlayerNames();
    $(document).mousemove(updateCursorCoordinates);

    cueBall.on('mousedown', startDrawing);
    $(document).on('mousemove', drawLine);
    $(document).on('mouseup', stopDrawing);

    function updatePlayerNames() {
        const player1Name = localStorage.getItem("player1_name") || "PLAYER 1";
        const player2Name = localStorage.getItem("player2_name") || "PLAYER 2";
        $("#player").text(`PLAYER 1: ${player1Name} | PLAYER 2: ${player2Name}`);
    }

    function updateCursorCoordinates(event) {
        $('#valx, #valy').remove();
        const { pageX: x, pageY: y } = event;
        $("<div>").attr("id", "valx").text(x).appendTo("#x");
        $("<div>").attr("id", "valy").text(y).appendTo("#y");
    }

    function startDrawing(e) {
        e.preventDefault();
        const svg = $(this).closest('svg').get(0);
        const point = getSVGCoordinates(svg, e.clientX, e.clientY);
        currentLine = createLine(point.x, point.y, point.x, point.y).appendTo(svg);
        isDragging = true;
    }

    function drawLine(e) {
        if (!isDragging || !currentLine) return;
        const svg = cueBall.closest('svg').get(0);
        const point = getSVGCoordinates(svg, e.clientX, e.clientY);
        currentLine.attr({ x2: point.x, y2: point.y });
    }

    function stopDrawing(e) {
        if (!isDragging || !currentLine) return;
        isDragging = false;
        const svg = cueBall.closest('svg').get(0);
        const point = getSVGCoordinates(svg, e.clientX, e.clientY);
        sendLineData(currentLine.attr('x1'), currentLine.attr('y1'), point.x, point.y);
        currentLine.remove();
        currentLine = null;
    }

    function getSVGCoordinates(svg, x, y) {
        const pt = svg.createSVGPoint();
        pt.x = x; pt.y = y;
        return pt.matrixTransform(svg.getScreenCTM().inverse());
    }

    function sendLineData(x1, y1, x2, y2) {
        var strokeDetails = {x1: x1, y1: y1, x2: x2, y2: y2};
        $.ajax({
            type: "POST",
            url: "/send-data",
            data: JSON.stringify(strokeDetails),
            contentType: "application/json",
            success: function() { console.log("Data sent successfully");
            animateSVGs();
            },
            error: function() {console.log("Error sending data"); }
        });
    }

    function createLine(x1, y1, x2, y2) {
        return $(document.createElementNS('http://www.w3.org/2000/svg', 'line'))
            .attr({ x1, y1, x2, y2, stroke: 'black', 'stroke-width': '15' });
    }

    let svgIndex = 0;

    function animateSVGs() {
        $("#pool-table").load(`/table-${svgIndex}.svg`, function(response, status, xhr) {
            if (status == "error") {
                console.log("Failed to load or no more SVGs");
                return;
            }
            
            svgIndex++;
            setTimeout(animateSVGs, 5); //5 ms 
        });
    }
});
