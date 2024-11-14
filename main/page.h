#ifndef PAGE_H
#define PAGE_H

const char PAGE_MAIN[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
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
      <option value="45">45</optiosn>
      <option value="50">50</option>
      <option value="55">55</option>
    </select>
    <button id="btn_add" class="add_btn">+</button>
  </div>

  <ul id="feedlist" class="feeding_time_list"></ul>
</body>
<script>
  // Variables globales
  var id = 0;
  var current_vals = [];

  function get_time_data() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "/timedata")
    xhttp.send();

    xhttp.onload = function () {
      if (xhr.status != 200) { 
        alert(`Error ${xhr.status}: ${xhr.statusText}`);
      } else { 
        alert(`Hecho, obtenidos ${xhr.response.length} bytes`); 
      }
    };

  }

  function get_current_time() {
    // Obtener los valores de la hora y minuto seleccionados
    const minute_select = document.getElementById("minute");
    const hour_select = document.getElementById("hour");

    // Formar el tiempo en formato HH:MM
    return hour_select.value + ":" + minute_select.value;
  }

  function send_time() {
    var xhttp = new XMLHttpRequest();

    const current_time = get_current_time();
    xhttp.open("PUT", "/time?VAL=" + current_time);
    xhttp.send();
  }

  // Función para eliminar un elemento de la lista
  function delete_list_el(htmlid, time) {
    // Obtener el contenedor de la lista y el elemento a eliminar
    const feedlist = document.getElementById("feedlist");
    const el = document.getElementById(htmlid);

    // Remover elemento de la lista de repetidos
    current_vals.splice(current_vals.findIndex((found) => found == time), 1);
    id--;

    // Eliminar el elemento de la lista
    feedlist.removeChild(el);

    var xhttp = new XMLHttpRequest();
    xhttp.open("PUT", "/delete?VAL="+time);
    xhttp.send();

  }

  // Función para manejar el evento de agregar un nuevo item
  function btn_add_event() {
    // Limitar a un máximo de 4 elementos
    if (id >= 4) {
      return;
    }

    var current_time = get_current_time();

    // Evitar duplicados en la lista
    if (current_vals.includes(current_time)) {
      return;
    }

    // Agregar el tiempo actual a la lista de valores
    current_vals.push(current_time);

    // Crear los elementos HTML para la nueva lista
    const feedlist = document.getElementById("feedlist");
    const node = document.createElement("li");
    const delete_btn = document.createElement("button");

    // Asignar atributos a los elementos
    node.className = "time_li";
    node.id = "time_li_" + id;
    id++; // Incrementar el id para el próximo elemento

    delete_btn.className = "delete_btn";
    delete_btn.innerHTML = "Eliminar";

    // Agregar el evento al botón de eliminar
    delete_btn.addEventListener("click", () => {
      delete_list_el(node.id, current_time);
    });

    // Añadir el botón de eliminar y el texto con la hora al nodo de la lista
    node.appendChild(delete_btn);
    const textNode = document.createTextNode(current_time);
    node.appendChild(textNode);

    // Añadir el nuevo nodo a la lista en el HTML
    feedlist.appendChild(node);
    send_time();
  }

  // Asociar el evento del botón de agregar con la función
  var btn_add = document.getElementById("btn_add");
  btn_add.addEventListener("click", btn_add_event);
</script>
<style>
  :root {
    --principal: #92aaef;
    --secundario: #ffd63471;
    --principal-dark: #242254;
  }

  * {
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  }

  h1 {
    font-size: 2.3em;
  }

  h3 {
    font-size: 1.8em;
  }

  body {
    background-color: var(--secundario);
    padding: 0 2em;
    display: flex;
    text-align: center;
    flex-direction: column;
    color: var(--principal-dark);
  }

  button {
    background-color: var(--principal);
    border: none;
    font-size: 1.4em;
    border-top: 1px solid var(--principal-dark);
    border-left: 1px solid var(--principal-dark);
    border-bottom: 3px solid var(--principal-dark);
    border-right: 3px solid var(--principal-dark);
    border-radius: .7em;

    padding: 1em;
    font-weight: 600;
    color: var(--principal-dark);

    display: flex;
    justify-content: center;
    align-items: center;

    width: 100%;
  }

  button:active {
    border-top: 3px solid var(--principal-dark);
    border-left: 3px solid var(--principal-dark);
    border-right: 1px solid var(--principal-dark);
    border-bottom: 1px solid var(--principal-dark);
  }

  select::-ms-expand {
    display: none;
  }


  select {
    -moz-appearance: none;
    -webkit-appearance: none;

    border-top: 1px solid var(--principal-dark);
    border-left: 1px solid var(--principal-dark);
    border-bottom: 3px solid var(--principal-dark);
    border-right: 3px solid var(--principal-dark);

    padding: .4em 1em;
    font-weight: 600;
    font-size: 1.2em;
    border-radius: 1.4em;
    height: 50px;
    background-color: var(--secundario);
  }

  select:active {
    border-top: 3px solid var(--principal-dark);
    border-left: 3px solid var(--principal-dark);
    border-right: 1px solid var(--principal-dark);
    border-bottom: 1px solid var(--principal-dark);
  }

  .horario_comida {
    display: flex;
    height: 50px;
    align-items: center;
    justify-content: center;
    gap: 25px;
  }

  .add_btn {
    height: 50px;
    width: 50px;
    padding: 0;
    border-radius: 100%;
    font-size: 1.4em;
  }

  .title {
    width: 100%;
  }

  .time_li {
    list-style: none;
    font-size: 1.4em;
    font-weight: 600;
    display: flex;
    justify-content: space-between;
    align-items: center;
    flex-direction: row-reverse;
  }

  .feeding_time_list {
    display: flex;
    flex-direction: column;
    margin: 0;
    padding: 0;
    padding: 1em 0;
    gap: 1em;
  }

  .delete_btn {
    font-size: .7em;
    width: auto;
    background-color: rgb(255, 93, 93);
    color: #000;

    border-top: 1px solid black;
    border-left: 1px solid black;
    border-bottom: 3px solid black;
    border-right: 3px solid black;
  }

  .delete_btn:active {
    border-top: 3px solid black;
    border-left: 3px solid black;
    border-bottom: 1px solid black;
    border-right: 1px solid black;
  }
</style>

</html>)=====";

#endif