#include "alphaBetaAI.h"
#include <iostream>
#include <utility>
#include <algorithm>

alphaBetaAI::alphaBetaAI(int maxDepth) :maxDepth(maxDepth), nodesChecked(0) {}

int alphaBetaAI::chooseMove(gameTree tree) {
	// reset nodesChecked counter
	nodesChecked = 0;

	// find best move
	move bestMove = evalualate(tree.root, maxDepth, -inf, inf);

	// print nodes checked in process
	std::cout << "Searching for best move: " << nodesChecked
		<< " node(s) checked" << std::endl;

	// return best move
	return bestMove.index;
}

move alphaBetaAI::evalualate(
	node currentNode, int depthToSearch, float alpha, float beta) {
	// increment number of nodes checked
	nodesChecked++;

	move bestMove;

	// if bottom of searchable tree, return node value
	if (depthToSearch == 0 || currentNode.isTerminal()) {
		bestMove.value = currentNode.getValue();
		return bestMove;
	}

	// get children of current node
	std::map<int, node> children = currentNode.getChildren();

	// if the current node is a max node
	if (currentNode.isMaxNode()) {
		float value = -inf;

		// for each possible move
		for (std::pair<int, node> child : children) {
			// check the expected value fo the move
			float checkValue = evalualate(
				child.second, depthToSearch - 1, alpha, beta).value;

			// if expected value is better than current best
			// set that move (and value) as new best
			if (checkValue > value) {
				value = checkValue;
				bestMove = { child.first, value };
			}

			// update alpha if appropriate
			alpha = std::max(alpha, value);

			// prune current node if possible
			if (alpha >= beta) break;
		}
	}
	// if the curren node is a min node
	else {
		float value = inf;

		// for each possible move
		for (std::pair<int, node> child : children) {
			// check the expected value fo the move
			float checkValue = evalualate(
				child.second, depthToSearch - 1, alpha, beta).value;

			// if expected value is better than current best
			// set that move (and value) as new best
			if (checkValue < value) {
				value = checkValue;
				bestMove = { child.first, value };
			}

			// update beta if appropriate
			beta = std::max(beta, value);

			// prune current node if possible
			if (alpha >= beta) break;
		}
	}
	
	return bestMove;
}