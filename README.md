# cbbPredictor
This code uses the Jeff Sagarin ELO-method rankings of the 300+ teams to help differentiate between the rankings.  These rankings' predictive power is often affected negatively by the winners of conference championships, plus they are too discrete to provide real predictions.  Once every team is assigned a weight based on thier ELO ranking, I can simply adjust the "upset probability" to try and best match the historical numbers of upsets for each round.

There is some inherent randomness in this code - i.e. it throws weighted dice to decide each winner, so a #1 seed always wins in the first round (as per histoy), but if a 5/12 matchup is close in ELO ranking, the upset is more likely.

You need ROOT (https://root.cern.ch/drupal/) to run this code, but I can adjust it (in another github branch) to use basic c++, but then I have to strip out the graphical debug output.
