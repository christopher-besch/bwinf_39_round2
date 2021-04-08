import { Lake } from "./lake_objects.js";
// global variables
let lake_canvas = document.getElementById("lake_canvas");
let lake_ctx = lake_canvas.getContext("2d");
lake_canvas.width = lake_canvas.scrollWidth;
lake_canvas.height = lake_canvas.scrollHeight;
let lake = new Lake(lake_ctx);
function render() {
    lake_ctx.clearRect(0, 0, lake_ctx.canvas.width, lake_ctx.canvas.height);
    lake.draw();
}
function update_settings() {
    // read form
    let lake_radius_raw = document.getElementById("lake-radius-form").value;
    let icon_size_raw = document.getElementById("icon-size-form").value;
    let start_angle_degree_raw = document.getElementById("start-angle-degree-form").value;
    let circumference_raw = document.getElementById("circumference-form").value;
    let houses_raw = document.getElementById("houses-form").value;
    let test_ices_raw = document.getElementById("test-ices-form").value;
    let check_ices_raw = document.getElementById("check-ices-form").value;
    let houses_labels = document.getElementById("houses-labels-form").checked;
    let test_ice_labels = document.getElementById("test-ice-labels-form").checked;
    let check_ice_labels = document.getElementById("check-ice-labels-form").checked;
    // convert to int
    let radius = parseInt(lake_radius_raw);
    let icon_size = parseInt(icon_size_raw);
    let circumference = parseInt(circumference_raw);
    let start_angle = (parseInt(start_angle_degree_raw) * Math.PI) / 180;
    // convert to int arrays
    let houses_str = !houses_raw ? [] : houses_raw.split(" ");
    let houses = [];
    for (let idx = 0; idx < houses_str.length; idx++) {
        let address = parseInt(houses_str[idx]);
        if (isNaN(address)) {
            alert("Invalid House Address!");
            return;
        }
        houses.push(address);
    }
    let test_ices_str = !test_ices_raw ? [] : test_ices_raw.split(" ");
    let test_ices = [];
    for (let idx = 0; idx < test_ices_str.length; idx++) {
        let address = parseInt(test_ices_str[idx]);
        if (isNaN(address)) {
            alert("Invalid Test Ice Address!");
            return;
        }
        test_ices.push(address);
    }
    let check_ices_str = !check_ices_raw ? [] : check_ices_raw.split(" ");
    let check_ices = [];
    for (let idx = 0; idx < check_ices_str.length; idx++) {
        let address = parseInt(check_ices_str[idx]);
        if (isNaN(address)) {
            alert("Invalid Check Ice Address!");
            return;
        }
        check_ices.push(address);
    }
    if (isNaN(radius) || isNaN(icon_size) || isNaN(circumference) || isNaN(start_angle) || houses.length == 0) {
        alert("Invalid Input!");
        return;
    }
    lake.update_positions(houses, test_ices, check_ices);
    lake.update(radius, icon_size, start_angle, circumference, houses_labels, test_ice_labels, check_ice_labels);
    render();
}
function download(open_in_new_tab = false) {
    let image_url = lake_canvas.toDataURL("lake.png");
    // create temporary link
    let tmp_link = document.createElement("a");
    if (open_in_new_tab)
        tmp_link.target = "_blank";
    else
        tmp_link.download = "lake.png";
    tmp_link.href = image_url;
    document.body.appendChild(tmp_link);
    tmp_link.click();
    document.body.removeChild(tmp_link);
}
//# sourceMappingURL=script.js.map