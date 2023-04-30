var items = document.querySelectorAll('.circle a');

for (var i = 0, l = items.length; i < l; i++) {
    items[i].style.left = (50 - 35 * Math.cos(-0.5 * Math.PI - 2 * (1 / l) * i * Math.PI)).toFixed(3) + "%";
    items[i].style.top = (50 + 35 * Math.sin(-0.5 * Math.PI - 2 * (1 / l) * i * Math.PI)).toFixed(3) + "%";
}

document.querySelector('.menu-button').onclick = function (e) {
    e.preventDefault(); document.querySelector('.circle').classList.toggle('open');
}