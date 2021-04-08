"use strict";
class Position {
    constructor(ctx, middle_x, middle_y, address, color, text_y_location_rel) {
        this.radius = 0;
        this.circumference = 0;
        this.icon_size = 0;
        this.text_y_location = 0;
        this.start_angle = 0;
        this.ctx = ctx;
        this.middle_x = middle_x;
        this.middle_y = middle_y;
        this.address = address;
        this.color = color;
        this.text_y_location_rel = text_y_location_rel;
    }
    resize(radius, icon_size, start_angle, circumference) {
        this.radius = radius;
        this.circumference = circumference;
        this.icon_size = icon_size;
        this.text_y_location = this.text_y_location_rel * icon_size;
        this.start_angle = start_angle;
    }
    draw_address() {
        this.ctx.font = `${this.icon_size * 3}px Sans`;
        this.ctx.textAlign = "center";
        this.ctx.fillStyle = this.color;
        this.ctx.fillText(this.address.toString(), 0, this.icon_size / 2);
    }
    // draw representation
    draw() {
        let angle_per_address = (Math.PI * 2) / this.circumference;
        // angle of this position
        let rotation = angle_per_address * this.address - this.start_angle;
        // location of this position
        let x = this.middle_x + Math.cos(rotation) * this.radius;
        let y = this.middle_y + Math.sin(rotation) * this.radius;
        // draw with certain rotation in certain location
        this.ctx.save();
        this.ctx.translate(x, y);
        // up for draw_icon should be away from the circle
        this.ctx.rotate(rotation - Math.PI / 2);
        this.draw_icon();
        // further away from lake but horizontally rotated
        this.ctx.translate(0, this.text_y_location);
        this.ctx.rotate(-rotation + Math.PI / 2);
        this.draw_address();
        this.ctx.restore();
    }
}
class TestIceCream extends Position {
    constructor(ctx, middle_x, middle_y, address) {
        super(ctx, middle_x, middle_y, address, "blue", -4);
    }
    draw_icon() {
        this.ctx.beginPath();
        this.ctx.moveTo(-this.icon_size, -this.icon_size);
        this.ctx.lineTo(this.icon_size, this.icon_size);
        this.ctx.moveTo(-this.icon_size, this.icon_size);
        this.ctx.lineTo(this.icon_size, -this.icon_size);
        this.ctx.strokeStyle = this.color;
        this.ctx.lineWidth = this.icon_size / 2;
        this.ctx.stroke();
    }
}
class CheckIceCream extends Position {
    constructor(ctx, middle_x, middle_y, address) {
        super(ctx, middle_x, middle_y, address, "blue", -4);
    }
    draw_icon() {
        this.ctx.beginPath();
        this.ctx.arc(0, 0, this.icon_size, 0, Math.PI * 2);
        this.ctx.strokeStyle = this.color;
        this.ctx.lineWidth = this.icon_size / 2;
        this.ctx.stroke();
    }
}
class House extends Position {
    constructor(ctx, middle_x, middle_y, address) {
        super(ctx, middle_x, middle_y, address, "black", 4);
    }
    draw_icon() {
        this.ctx.fillStyle = this.color;
        this.ctx.fillRect(-this.icon_size / 2, -this.icon_size, this.icon_size, this.icon_size * 2);
    }
}
class Lake {
    constructor(ctx) {
        this.circumference = 0;
        this.radius = 0;
        this.icon_size = 0;
        this.start_angle = 0;
        // used to check if the addresses have been updated
        this.house_addresses = [];
        this.test_ices_addresses = [];
        this.check_ice_addresses = [];
        this.houses = [];
        this.test_ices = [];
        this.check_ices = [];
        this.ctx = ctx;
        this.x = ctx.canvas.width / 2;
        this.y = ctx.canvas.height / 2;
    }
    resize(radius, icon_size, start_angle, circumference) {
        this.radius = radius;
        this.circumference = circumference;
        this.icon_size = icon_size;
        this.start_angle = start_angle;
        this.houses.forEach((position) => {
            position.resize(radius, icon_size, start_angle, circumference);
        });
        this.test_ices.forEach((position) => {
            position.resize(radius, icon_size, start_angle, circumference);
        });
        this.check_ices.forEach((position) => {
            position.resize(radius, icon_size, start_angle, circumference);
        });
    }
    update_positions(house_addresses, test_ice_addresses, check_ice_addresses) {
        // overwrite if addresses have been updated
        if (house_addresses != this.house_addresses) {
            this.houses = [];
            house_addresses.forEach((address) => {
                this.houses.push(new House(this.ctx, this.x, this.y, address));
            });
        }
        if (test_ice_addresses != this.test_ices_addresses) {
            this.test_ices = [];
            test_ice_addresses.forEach((address) => {
                this.test_ices.push(new TestIceCream(this.ctx, this.x, this.y, address));
            });
        }
        if (check_ice_addresses != this.check_ice_addresses) {
            this.check_ice_addresses = [];
            check_ice_addresses.forEach((address) => {
                this.check_ices.push(new CheckIceCream(this.ctx, this.x, this.y, address));
            });
        }
        this.house_addresses = house_addresses;
        this.test_ices_addresses = test_ice_addresses;
        this.check_ice_addresses = check_ice_addresses;
    }
    draw() {
        // draw lake
        this.ctx.beginPath();
        this.ctx.arc(this.ctx.canvas.width / 2, this.ctx.canvas.height / 2, this.radius, 0, Math.PI * 2);
        // this.ctx.fillStyle = "#ffffff";
        // this.ctx.fill();
        this.ctx.lineWidth = 1;
        this.ctx.strokeStyle = "0d0d0d";
        this.ctx.stroke();
        // draw objects
        this.houses.forEach((position) => {
            position.draw();
        });
        this.test_ices.forEach((position) => {
            position.draw();
        });
        this.check_ices.forEach((position) => {
            position.draw();
        });
    }
}
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
    let lake_radius_form = document.getElementById("lake-radius-form");
    let icon_size_form = document.getElementById("icon-size-form");
    let start_angle_degree_form = document.getElementById("start-angle-degree-form");
    let circumference_form = document.getElementById("circumference-form");
    let houses_form = document.getElementById("houses-form");
    let test_ices_form = document.getElementById("test-ices-form");
    let check_ices_form = document.getElementById("check-ices-form");
    // convert to int
    let radius = parseInt(lake_radius_form.value);
    let icon_size = parseInt(icon_size_form.value);
    let circumference = parseInt(circumference_form.value);
    let start_angle = (parseInt(start_angle_degree_form.value) * Math.PI) / 180;
    // convert to int arrays
    let houses_str = !houses_form.value ? [] : houses_form.value.split(" ");
    let houses = [];
    for (let idx = 0; idx < houses_str.length; idx++) {
        let address = parseInt(houses_str[idx]);
        if (isNaN(address)) {
            alert("Invalid House Address!");
            return;
        }
        houses.push(address);
    }
    let test_ices_str = !test_ices_form.value ? [] : test_ices_form.value.split(" ");
    let test_ices = [];
    for (let idx = 0; idx < test_ices_str.length; idx++) {
        let address = parseInt(test_ices_str[idx]);
        if (isNaN(address)) {
            alert("Invalid Test Ice Address!");
            return;
        }
        test_ices.push(address);
    }
    let check_ices_str = !check_ices_form.value ? [] : check_ices_form.value.split(" ");
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
    lake.resize(radius, icon_size, start_angle, circumference);
    render();
}
function download() {
    let image_url = lake_canvas.toDataURL("lake.png");
    // create temporary link
    let tmp_link = document.createElement("a");
    tmp_link.download = "lake.png";
    tmp_link.href = image_url;
    document.body.appendChild(tmp_link);
    tmp_link.click();
    document.body.removeChild(tmp_link);
}
//# sourceMappingURL=script.js.map