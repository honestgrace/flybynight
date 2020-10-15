// tsc .\src\index.ts --outdir .\
import { Point } from './Point';
let p = new Point({ x: 5, y: 3 });
let p2 = new Point({ x: 5, y: 3 });
p.draw();
p.getDistance(p2);
p.x = 555;
console.log(p.x);
