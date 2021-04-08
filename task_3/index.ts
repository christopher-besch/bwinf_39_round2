abstract class Position {
    protected ctx: CanvasRenderingContext2D;
    protected middle_x: number;
    protected middle_y: number;
    // position on circle
    protected address: number;
    protected color: string;
    protected radius = 0;
    protected circumference = 0;
    protected icon_size = 0;
    // relative to icon_size
    protected text_y_location_rel: number;
    protected text_y_location = 0;
    protected start_angle = 0;
    protected labels = false;

    constructor(
        ctx: CanvasRenderingContext2D,
        middle_x: number,
        middle_y: number,
        address: number,
        color: string,
        text_y_location_rel: number
    ) {
        this.ctx = ctx;
        this.middle_x = middle_x;
        this.middle_y = middle_y;
        this.address = address;
        this.color = color;
        this.text_y_location_rel = text_y_location_rel;
    }

    update(radius: number, icon_size: number, start_angle: number, circumference: number, labels: boolean): void {
        this.radius = radius;
        this.circumference = circumference;
        this.icon_size = icon_size;
        this.text_y_location = this.text_y_location_rel * icon_size;
        this.start_angle = start_angle;
        this.labels = labels;
    }

    // icon should be scalable with icon_size
    abstract draw_icon(): void;

    draw_label(): void {
        this.ctx.font = `${this.icon_size * 3}px Sans`;
        this.ctx.textAlign = "center";
        this.ctx.fillStyle = this.color;
        this.ctx.fillText(this.address.toString(), 0, this.icon_size / 2);
    }

    // draw representation
    draw(): void {
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

        if (this.labels) {
            // further away from lake but horizontally rotated
            this.ctx.translate(0, this.text_y_location);
            this.ctx.rotate(-rotation + Math.PI / 2);
            this.draw_label();
        }

        this.ctx.restore();
    }
}

class TestIceCream extends Position {
    constructor(ctx: CanvasRenderingContext2D, middle_x: number, middle_y: number, address: number) {
        super(ctx, middle_x, middle_y, address, "blue", -4);
    }

    draw_icon(): void {
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
    constructor(ctx: CanvasRenderingContext2D, middle_x: number, middle_y: number, address: number) {
        super(ctx, middle_x, middle_y, address, "blue", -4);
    }

    draw_icon(): void {
        this.ctx.beginPath();
        this.ctx.arc(0, 0, this.icon_size, 0, Math.PI * 2);
        this.ctx.strokeStyle = this.color;
        this.ctx.lineWidth = this.icon_size / 2;
        this.ctx.stroke();
    }
}

class House extends Position {
    constructor(ctx: CanvasRenderingContext2D, middle_x: number, middle_y: number, address: number) {
        super(ctx, middle_x, middle_y, address, "black", 4);
    }

    draw_icon(): void {
        this.ctx.fillStyle = this.color;
        this.ctx.fillRect(-this.icon_size / 2, -this.icon_size, this.icon_size, this.icon_size * 2);
    }
}

class Lake {
    private ctx: CanvasRenderingContext2D;
    private radius = 0;
    private x: number;
    private y: number;
    // used to check if the addresses have been updated
    private house_addresses: number[] = [];
    private test_ices_addresses: number[] = [];
    private check_ice_addresses: number[] = [];
    private houses: House[] = [];
    private test_ices: TestIceCream[] = [];
    private check_ices: CheckIceCream[] = [];

    constructor(ctx: CanvasRenderingContext2D) {
        this.ctx = ctx;
        this.x = ctx.canvas.width / 2;
        this.y = ctx.canvas.height / 2;
    }

    update(
        radius: number,
        icon_size: number,
        start_angle: number,
        circumference: number,
        house_labels: boolean,
        test_ice_label: boolean,
        check_ice_labels: boolean
    ): void {
        this.radius = radius;

        this.houses.forEach((position) => {
            position.update(radius, icon_size, start_angle, circumference, house_labels);
        });
        this.test_ices.forEach((position) => {
            position.update(radius, icon_size, start_angle, circumference, test_ice_label);
        });
        this.check_ices.forEach((position) => {
            position.update(radius, icon_size, start_angle, circumference, check_ice_labels);
        });
    }

    update_positions(house_addresses: number[], test_ice_addresses: number[], check_ice_addresses: number[]): void {
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
            this.check_ices = [];
            check_ice_addresses.forEach((address) => {
                this.check_ices.push(new CheckIceCream(this.ctx, this.x, this.y, address));
            });
        }
        this.house_addresses = house_addresses;
        this.test_ices_addresses = test_ice_addresses;
        this.check_ice_addresses = check_ice_addresses;
    }

    draw(): void {
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
let lake_canvas = document.getElementById("lake_canvas") as HTMLCanvasElement;
let lake_ctx = lake_canvas.getContext("2d") as CanvasRenderingContext2D;
lake_canvas.width = lake_canvas.scrollWidth;
lake_canvas.height = lake_canvas.scrollHeight;
let lake = new Lake(lake_ctx);

function render(): void {
    lake_ctx.clearRect(0, 0, lake_ctx.canvas.width, lake_ctx.canvas.height);
    lake.draw();
}

function update_settings(): void {
    // read form
    let lake_radius_raw = (document.getElementById("lake-radius-form") as HTMLInputElement).value;
    let icon_size_raw = (document.getElementById("icon-size-form") as HTMLInputElement).value;
    let start_angle_degree_raw = (document.getElementById("start-angle-degree-form") as HTMLInputElement).value;
    let circumference_raw = (document.getElementById("circumference-form") as HTMLInputElement).value;

    let houses_raw = (document.getElementById("houses-form") as HTMLInputElement).value;
    let test_ices_raw = (document.getElementById("test-ices-form") as HTMLInputElement).value;
    let check_ices_raw = (document.getElementById("check-ices-form") as HTMLInputElement).value;

    let houses_labels = (document.getElementById("houses-labels-form") as HTMLInputElement).checked;
    let test_ice_labels = (document.getElementById("test-ice-labels-form") as HTMLInputElement).checked;
    let check_ice_labels = (document.getElementById("check-ice-labels-form") as HTMLInputElement).checked;

    // convert to int
    let radius = parseInt(lake_radius_raw);
    let icon_size = parseInt(icon_size_raw);
    let circumference = parseInt(circumference_raw);
    let start_angle = (parseInt(start_angle_degree_raw) * Math.PI) / 180;

    // convert to int arrays
    let houses_str = !houses_raw ? [] : houses_raw.split(" ");
    let houses: number[] = [];
    for (let idx = 0; idx < houses_str.length; idx++) {
        let address = parseInt(houses_str[idx]);
        if (isNaN(address)) {
            alert("Invalid House Address!");
            return;
        }
        houses.push(address);
    }
    let test_ices_str = !test_ices_raw ? [] : test_ices_raw.split(" ");
    let test_ices: number[] = [];
    for (let idx = 0; idx < test_ices_str.length; idx++) {
        let address = parseInt(test_ices_str[idx]);
        if (isNaN(address)) {
            alert("Invalid Test Ice Address!");
            return;
        }
        test_ices.push(address);
    }
    let check_ices_str = !check_ices_raw ? [] : check_ices_raw.split(" ");
    let check_ices: number[] = [];
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

function download(open_in_new_tab: boolean = false): void {
    let image_url = lake_canvas.toDataURL("lake.png");

    // create temporary link
    let tmp_link = document.createElement("a");
    if (open_in_new_tab) tmp_link.target = "_blank";
    else tmp_link.download = "lake.png";
    tmp_link.href = image_url;
    document.body.appendChild(tmp_link);
    tmp_link.click();
    document.body.removeChild(tmp_link);
}
