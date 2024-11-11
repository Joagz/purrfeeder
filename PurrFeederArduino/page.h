
const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
    <title>PurrFeeder Configurer</title>
</head>

<body>
    <div>
        <h1 class="title">PurrFeeder</h1>
        <button>Rellenar Plato</button>
    </div>

    <h3>Horario de comida</h3>
    <div class="horario_comida">
        <select name="" id="hour">
            <option value="00">00</option>
            <option value="01">01</option>
            <option value="02">02</option>
            <option value="03">03</option>
            <option value="04">04</option>
            <option value="05">05</option>
            <option value="06">06</option>
            <option value="07">07</option>
            <option value="08">08</option>
            <option value="09">09</option>
            <option value="10">10</option>
            <option value="11">11</option>
            <option value="12">12</option>
            <option value="13">13</option>
            <option value="14">14</option>
            <option value="15">15</option>
            <option value="16">16</option>
            <option value="17">17</option>
            <option value="18">18</option>
            <option value="19">19</option>
            <option value="20">20</option>
            <option value="21">21</option>
            <option value="22">22</option>
            <option value="23">23</option>
        </select>
        <select name="" id="minute">
            <option value="00">00</option>
            <option value="05">05</option>
            <option value="10">10</option>
            <option value="15">15</option>
            <option value="20">20</option>
            <option value="25">25</option>
            <option value="30">30</option>
            <option value="35">35</option>
            <option value="40">40</option>
            <option value="45">45</option>
            <option value="50">50</option>
            <option value="55">55</option>
        </select>
        <button id="btn_add" class="add_btn">+</button>
    </div>

    <ul id="feedlist" class="feeding_time_list">
    </ul>

</body>
<script>
    function btn_add_event() {
        const feedlist = document.getElementById("feedlist");
        const node = document.createElement("li");
        const delete_btn = document.createElement("button");
        node.className = "time_li";

        delete_btn.className = "delete_btn";
        delete_btn.innerHTML = "Eliminar";

        node.appendChild(delete_btn);

        const minute_select = document.getElementById("minute");
        const hour_select = document.getElementById("hour");
        const textNode = document.createTextNode(hour_select.value + ":" + minute_select.value);

        node.appendChild(textNode);
        feedlist.appendChild(node);
    }

    var btn_add = document.getElementById("btn_add");
    btn_add.addEventListener("click", btn_add_event);

</script>

</html>
)

)=====";

