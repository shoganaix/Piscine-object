/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Position.hpp"
#include "Statistic.hpp"
#include "Tool.hpp"
#include "Shovel.hpp"
#include "Hammer.hpp"
#include "Worker.hpp"
#include "Workshop.hpp"

# include <exception>		// std::exception, the outermost catch
# include <iostream>		// std::cout, std::cerr, std::endl
# include <stdexcept>		// std::invalid_argument, documented in the refused operations below


/* Everything that can be refused is refused by an exception, and every refusal is
 * caught here, so the demonstration always runs to the end.
 *
 * The trace printed below is the deliverable the evaluation reads: the subject asks
 * for an output in pretty much every function, method, constructor, operator and
 * destructor, and asks that those outputs show in which order the code runs.
 * Each block therefore maps to one section of the subject, and the block titles are
 * the section numbers, so any line of the output can be traced back to the code.
 *
 * Every worker and every workshop below lives in its own block, so the destructors
 * run at a readable place in the trace instead of all at the end of main.
 */
int	main()
{
	try		// every operation below may throw, so guard the whole thing
	{

/* ==========================================================================================
 * IV.1  COMPOSITION
 * ==========================================================================================
 * A worker IS made of a position and a statistic, stored BY VALUE inside him.
 * Both members are built BEFORE the worker constructor body runs, in the order they are
 * declared, and the destruction is the exact reverse once the worker is gone.
 * Neither of the two can be NULL, swapped, or shared with another worker.
 *
 * ! Note: every worker below is built from two NAMED locals rather than from temporaries.
 * ! In C++98 the order in which the arguments of a constructor call are evaluated is
 * ! NOT specified, so a temporary would sometimes print before the other one and the
 * ! trace would show the composition backwards from one block to the next.
 */
		std::cout << "=== IV.1 Composition ===" << std::endl;
		{
			Position	alicePosition(1, 2, 3);
			Statistic	aliceStat(3, 250);
			// Both members are built by the worker constructor itself, that is composition:
			// he does not receive them, he builds them, and they cannot live without him
			Worker		alice(alicePosition, aliceStat);

			std::cout << "Alice is " << alice.getLabel() << ", level " << alice.getStat().level
						<< ", " << alice.getStat().exp << " exp" << std::endl;
			std::cout << "She is handed " << alice.getNbTools() << " tool(s) and joined "
						<< alice.getNbWorkshops() << " workshop(s): a worker starts with nothing" << std::endl;
		}	// <- alice dies here, and her position and her statistic die with her, there is
			//    nothing left of them and nobody can reach them any more


/* ==========================================================================================
 * IV.2  AGGREGATION
 * ==========================================================================================
 * The shovel is created HERE, outside of any worker, which is what makes main its
 * owner. A worker only ever receives a POINTER to it, so he may vanish while holding
 * one and the tool survives him. That is the whole difference with the composition above.
 */
		std::cout << std::endl << "=== IV.2 Aggregation ===" << std::endl;

		Shovel	shovel;		// main owns this one, it is not attached to anybody yet
		{
			Position	bobPosition(4, 5, 6);
			Statistic	bobStat(1, 10);
			Worker		bob(bobPosition, bobStat);
			bob.giveTool(&shovel);		// bob BORROWS the shovel, he does not own it
			std::cout << "Bob holds " << bob.getNbTools() << " tool(s)" << std::endl;
			shovel.use();					// the worker is not the only one who can use it
		}	// <- bob dies here. Look at his destructor trace: the shovel is NOT destroyed,
			//    only left behind. Composition would have destroyed it.

		std::cout << "The shovel outlived the worker, uses so far : " << shovel.getNumberOfUses() << std::endl;
		std::cout << "Nobody holds it any more, the tool is back on the shelf" << std::endl;

		// Giving it to somebody else must remove it from the first holder
		{
			Position	carolPosition(7, 8, 9);
			Statistic	carolStat(2, 20);
			Position	davePosition(0, 0, 0);
			Statistic	daveStat(1, 5);
			Worker		carol(carolPosition, carolStat);
			Worker		dave(davePosition, daveStat);

			carol.giveTool(&shovel);
			dave.giveTool(&shovel);		// the shovel is stolen from carol

			std::cout << "Carol is left with " << carol.getNbTools() << " tool(s)" << std::endl;
			std::cout << "Dave is left with  " << dave.getNbTools() << " tool(s)" << std::endl;
		}	// dave dies still holding the shovel, and it survives him a second time


/* ==========================================================================================
 * IV.3  INHERITANCE
 * ==========================================================================================
 * Shovel and Hammer both derive from the abstract Tool and share nothing but their
 * behaviour. The worker stores Tool pointers only, so the same toolbox holds both, and
 * the same call reaches the right implementation without the caller ever knowing.
 */
		std::cout << std::endl << "=== IV.3 Inheritance ===" << std::endl;

		Hammer	hammer;
		{
			Position	erinPosition(5, 5, 5);
			Statistic	erinStat(4, 400);
			Worker		erin(erinPosition, erinStat);
			erin.giveTool(&hammer);
			erin.giveTool(&shovel);		// several tools at the same time
			std::cout << "Erin holds " << erin.getNbTools() << " tool(s) at once" << std::endl;

			/* The exact same virtual call, on an array the worker knows nothing about.
			 * The loop does not know which tool it holds, it only knows Tool, and yet
			 * each element behaves as itself. That is what the vtable buys.
			 */
			Tool*	tools[2] = { &hammer, &shovel };
			for (int i = 0; i < 2; i++)
				tools[i]->use();

			// The tools can be given back one by one, the others stay put
			erin.takeTool(&hammer);
			std::cout << "Erin still holds " << erin.getNbTools() << " tool(s) after giving the hammer back" << std::endl;
		}


/* ==========================================================================================
 * IV.4  ASSOCIATION
 * ==========================================================================================
 * A workshop and its workers only point at each other. One worker may be inside
 * several workshops at once, and he may leave whenever he wants.
 */
		std::cout << std::endl << "=== IV.4 Association ===" << std::endl;

		{
			Workshop	forestCamp("forest camp");		// open to anybody
			Workshop	mineCamp("mine");				// open to anybody as well

			Position	frankPosition(1, 1, 1);
			Statistic	frankStat(7, 700);
			Worker		frank(frankPosition, frankStat);
			frank.giveTool(&shovel);

			// A worker who is registered nowhere has nothing to do.
			// ! Note: the call is made first and printed after, otherwise the trace of
			// ! work() lands in the middle of this line, since work() writes to the
			// ! same stream without flushing.
			const bool	workedBeforeSigningUp = frank.work();
			std::cout << "Frank works before signing up, work() returns : " << workedBeforeSigningUp << std::endl;

			// One worker, several workshops at the same time
			forestCamp.enrolWorker(&frank);
			mineCamp.enrolWorker(&frank);
			std::cout << "Frank is now inside " << frank.getNbWorkshops() << " workshops at once" << std::endl;
			std::cout << "isRegisteredTo(mineCamp) : " << frank.isRegisteredTo(&mineCamp) << std::endl;

			// Each workshop runs its own day for him
			forestCamp.executeWorkDay();
			mineCamp.executeWorkDay();

			// And he leaves freely, from one side only
			forestCamp.releaseWorker(&frank);
			std::cout << "Frank is down to " << frank.getNbWorkshops() << " workshop(s)" << std::endl;
		}	// frank dies, then the two workshops close: the destructor trace shows each side
			//    telling the other it is gone, so neither is left with a dangling pointer


/* ==========================================================================================
 * THE THREE BONUSES
 * ==========================================================================================
 * BONUS 1, the getTool<ToolType>() template, used above and below.
 * BONUS 2, a workshop that only accepts workers holding a given tool.
 * BONUS 3, a worker who loses that tool is released from the workshop on his own.
 */
		std::cout << std::endl << "=== Bonuses ===" << std::endl;

		{
			/* BONUS 2, a workshop that turns away anybody without a shovel */
			Workshop	kitchenGarden("kitchen garden", "shovel");

			Position	gracePosition(2, 2, 2);
			Statistic	graceStat(3, 30);
			Position	hugoPosition(3, 3, 3);
			Statistic	hugoStat(3, 33);
			Worker		grace(gracePosition, graceStat);
			Worker		hugo(hugoPosition, hugoStat);

			// Refused: she has no tool at all
			kitchenGarden.enrolWorker(&grace);
			// Accepted: she is holding the shovel the workshop asked for
			grace.giveTool(&shovel);
			kitchenGarden.enrolWorker(&grace);
			// Still refused: he is not holding one, and being in another workshop changes nothing
			kitchenGarden.enrolWorker(&hugo);

			// BONUS 1, asking for a tool by type
			Shovel*	p_shovel = grace.getTool<Shovel>();
			Hammer*	p_hammer = grace.getTool<Hammer>();		// she has none
			std::cout << "getTool<Shovel>() returned " << (p_shovel ? "her shovel" : "nothing")
						<< ", getTool<Hammer>() returned " << (p_hammer ? "a hammer" : "nothing") << std::endl;

			/* BONUS 3, she hands the shovel back and the workshop drops her by itself.
			 * Nobody calls releaseWorker() anywhere below, and she still leaves.
			 */
			grace.takeTool(&shovel);
			std::cout << "Grace is down to " << grace.getNbWorkshops()
						<< " workshop(s), the garden let her go on its own" << std::endl;

			// The shovel is on the shelf again, so Hugo can pick it up and be accepted
			hugo.giveTool(&shovel);
			kitchenGarden.enrolWorker(&hugo);
		}


/* ==========================================================================================
 * REFUSED OPERATIONS
 * ==========================================================================================
 * The subject requires that the program never crashes, so every misuse is a clean
 * exception, each caught on its own so the demonstration keeps going.
 */
		std::cout << std::endl << "=== Refused operations ===" << std::endl;

		// A negative coordinate
		try
		{
			const Position	impossible(-1, 0, 0);
			std::cout << "Never reached, but the compiler does not know it: " << impossible.x << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << "Caught : " << e.what() << std::endl;
		}

		// A negative level
		try
		{
			const Statistic	impossible(-1, 100);
			std::cout << "Never reached, but the compiler does not know it: " << impossible.level << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << "Caught : " << e.what() << std::endl;
		}

		// A workshop demanding a tool with no name
		try
		{
			Workshop	impossible("nameless", "");
			std::cout << "Never reached, but the compiler does not know it: " << impossible.getName() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << "Caught : " << e.what() << std::endl;
		}

		// Being handed nothing, and signing up nobody
		{
			Position	soloPosition(9, 9, 9);
			Statistic	soloStat(1, 1);
			Worker		solo(soloPosition, soloStat);

			try
			{
				solo.giveTool(0);			// a null tool is a programming mistake -> throws
				std::cout << "Never reached, but the compiler does not know it" << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "Caught : " << e.what() << std::endl;
			}

			// Giving back a tool that was never given, refused without an exception
			solo.takeTool(&shovel);

			try
			{
				Workshop	empty("empty room");
				empty.enrolWorker(0);		// a null worker is a programming mistake -> throws
				std::cout << "Never reached, but the compiler does not know it: " << empty.getNbWorkers() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "Caught : " << e.what() << std::endl;
			}
		}

		std::cout << std::endl << "The program reached the end without crashing" << std::endl;
	}
	// ANY exception not handled above
	catch (const std::exception& e)
	{
		std::cerr << "Unexpected error : " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
