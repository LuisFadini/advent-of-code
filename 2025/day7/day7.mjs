import { readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const sample1 = readFileSync(join(__dirname, "sample1.txt"), "utf8");
const input = readFileSync(join(__dirname, "input.txt"), "utf8");

/**
 *
 * @param {string} input
 */
function run_part1(input) {
  const startBeamPos = input.split("\n")[0].indexOf("S");
  const rows = input.split("\n");
  let beams = new Set([startBeamPos]);
  let splits = 0;

  for (let r = 1; r < rows.length; r++) {
    const row = rows[r];
    const nextBeams = new Set();

    for (const col of beams) {
      if (col < 0 || col >= row.length) continue;

      if (row[col] === "^") {
        splits++;
        nextBeams.add(col - 1);
        nextBeams.add(col + 1);
      } else {
        nextBeams.add(col);
      }
    }

    beams = nextBeams;
  }

  return splits;
}

/**
 *
 * @param {string} input
 */
function run_part2(input) {
  const rows = input.trim().split("\n");
  const cols = rows[0].length;

  let ways = Array(cols).fill(0n);
  let nextWays = Array(cols).fill(0n);

  const startCol = rows[0].indexOf("S");
  ways[startCol] = 1n;

  for (let r = 1; r < rows.length; r++) {
    nextWays.fill(0n);
    const row = rows[r];

    for (let c = 0; c < cols; c++) {
      if (ways[c] === 0n) continue;

      if (row[c] === "^") {
        if (c > 0) nextWays[c - 1] += ways[c];
        if (c < cols - 1) nextWays[c + 1] += ways[c];
      } else {
        nextWays[c] += ways[c];
      }
    }

    [ways, nextWays] = [nextWays, ways];
  }

  return ways.reduce((sum, v) => sum + v, 0n);
}

console.log(
  `Day 7 sample1.txt: part1: ${run_part1(sample1)} part2: ${run_part2(sample1)}`
);
console.log(
  `Day 7 input.txt: part1: ${run_part1(input)} part2: ${run_part2(input)}`
);

