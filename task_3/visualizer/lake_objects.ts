export abstract class Position {
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

export class TestIceCream extends Position {
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

export class CheckIceCream extends Position {
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

export class House extends Position {
    constructor(ctx: CanvasRenderingContext2D, middle_x: number, middle_y: number, address: number) {
        super(ctx, middle_x, middle_y, address, "black", 4);
    }

    draw_icon(): void {
        this.ctx.fillStyle = this.color;
        this.ctx.fillRect(-this.icon_size / 2, -this.icon_size, this.icon_size, this.icon_size * 2);
    }
}

export class Lake {
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
