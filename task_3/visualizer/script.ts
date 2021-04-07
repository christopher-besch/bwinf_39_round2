abstract class Position {
    ctx: CanvasRenderingContext2D;
    address: number;
    color: string;

    constructor(ctx: CanvasRenderingContext2D, address: number, color: string) {
        this.ctx = ctx;
        this.address = address;
        this.color = color;
    }

    abstract draw(x: number, y: number, rotation: number): void;

    draw_on_circle(middle_x: number, middle_y: number, radius: number, circumference: number, address: number): void {
        let angle_per_address = (Math.PI * 2) / circumference;
        let angle = angle_per_address * address;
        let x = Math.cos(angle) * radius;
        let y = Math.sin(angle) * radius;
        this.draw(x, y, angle);
    }
}

class IceCream extends Position {
    constructor(ctx: CanvasRenderingContext2D, address: number) {
        super(ctx, address, "black");
    }

    draw(x: number, y: number, rotation: number): void {
        console.log(`${x} ${y}`);
        this.ctx.save();
        this.ctx.translate(x, y);
        this.ctx.rotate(rotation);
        this.ctx.beginPath();
        this.ctx.moveTo(0, -10);
        this.ctx.lineTo(0, 10);
        this.ctx.fillStyle = this.color;
        this.ctx.fill();
        this.ctx.restore();
    }
}

class Lake {
    ctx: CanvasRenderingContext2D;
    circumference: number;
    radius: number;
    x: number;
    y: number;
    positions: Position[];

    constructor(ctx: CanvasRenderingContext2D) {
        this.ctx = ctx;
        this.circumference = 0;
        this.radius = (this.ctx.canvas.height - 100) / 2;
        this.x = this.ctx.canvas.width / 2;
        this.y = this.ctx.canvas.height / 2;
        this.positions = [];
    }

    draw(radius: number, circumference: number): void {
        this.ctx.clearRect(0, 0, this.ctx.canvas.width, this.ctx.canvas.height);
        this.radius = radius;
        this.circumference = circumference;
        this.x = this.ctx.canvas.width / 2;
        this.y = this.ctx.canvas.height / 2;

        // draw lake
        this.ctx.beginPath();
        this.ctx.arc(this.ctx.canvas.width / 2, this.ctx.canvas.height / 2, this.radius, 0, Math.PI * 2);
        this.ctx.fillStyle = "#8ED6FF";
        this.ctx.fill();
        this.ctx.lineWidth = 5;
        this.ctx.strokeStyle = "green";
        this.ctx.stroke();

        // draw objects
        this.positions.forEach((position) => {
            let angle = (Math.PI * 2) / this.circumference;
            position.draw_on_circle(this.x, this.y, this.radius, this.circumference, position.address);
        });
    }
}

let lake_canvas = document.getElementById("lake_canvas") as HTMLCanvasElement;
let lake_ctx = lake_canvas.getContext("2d") as CanvasRenderingContext2D;
lake_canvas.width = lake_canvas.scrollWidth;
lake_canvas.height = lake_canvas.scrollHeight;
let lake = new Lake(lake_canvas.getContext("2d") as CanvasRenderingContext2D);

function render() {
    let lake_radius_form = document.getElementById("lake-radius-form") as HTMLInputElement;
    let lake_radius = parseInt(lake_radius_form.value);
    let circumference_form = document.getElementById("circumference-form") as HTMLInputElement;
    let circumference = parseInt(circumference_form.value);
    lake.positions = [new IceCream(lake_ctx, 5)];
    lake.draw(lake_radius, circumference);
}
