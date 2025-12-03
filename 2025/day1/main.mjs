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
  let dial = 50;
  let possiblePasswords = 0;
  input.split("\n").forEach((l) => {
    if (l[0] == "L") dial -= Number(l.replace("L", ""));
    else if (l[0] == "R") dial += Number(l.replace("R", ""));

    if (dial <= 0) dial += 100;
    dial = (dial + 100) % 100;

    if (dial == 0) possiblePasswords++;
  });

  return possiblePasswords;
}

console.log(`Sample possible passwords: ${run_part1(sample1)}`);
console.log(`Possible passowrds: ${run_part1(input)}`);

