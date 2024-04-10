Introduction
The architecture of this simulation model is inspired by NetLogo, a programmable modeling environment for simulating natural and social phenomena. The model is designed to facilitate the creation and management of agents within a simulated world, allowing for the exploration of complex behaviors and interactions in a grid-based environment. The core components of this architecture are the World, Agent, Patch, and Turtle classes, with the World class serving as the central coordinator of the simulation.

Architecture Components
1. Agent (Abstract Class)
The Agent class is an abstract base class that provides foundational attributes and functionalities for all agents within the simulation, both stationary and mobile. Attributes like position (x, y), color, and opacity are defined here, along with accessors and mutators for these properties. This class is designed to be extended by more specific agent types to inherit these basic properties.

2. Patch
Derived from the Agent class, Patch represents a stationary agent that models a segment of the environment. Each Patch is a cell in the simulation grid and can hold environment variables (such as temperature or humidity) and track mobile agents (Turtle objects) that are currently located within it. Patches are primarily responsible for managing the environmental aspects of the simulation, including diffusion and evaporation processes.

3. Turtle (Absract Class)
Turtle extends the Agent class to represent mobile agents within the simulation. These agents can move across patches, interact with other agents, and modify environment variables. The Turtle class introduces additional attributes and methods related to mobility (such as heading and movement functions), along with simulation-specific variables. It is an abstract class, meant to be subclassed by specific types of mobile agents that require specialized behaviors or properties.

4. World
The World class acts as the main handler for the entire simulation. It maintains the grid of Patch objects and manages the collection of Turtle and other derived agent instances. Additionally, global simulation variables are maintained as part of World. The World is responsible for initializing the simulation environment, executing simulation steps (ticks), and applying global rules such as diffusion and evaporation. It uses a random number generator to support stochastic behaviors and processes within the simulation.

Functionality and Interactions
Initialization: At the start of the simulation, the World initializes itself by creating patches according to the defined grid size and populating the environment with Turtle agents and any other required entities.

Simulation Steps: Each simulation tick involves updating the states of agents based on their individual behaviors and interactions with their environment. The World class coordinates these updates, ensuring that each agent acts according to its defined rules.

Environmental Processes: Processes such as diffusion and evaporation are managed by the World class and affect the environment variables stored in Patch objects. These processes simulate natural phenomena, impacting the behavior and decisions of mobile agents.



Extensibility
This architecture is designed to be extensible, allowing for the introduction of new agent types and behaviors.

To introduce new types of mobile agents:

Subclassing Turtle: Begin by creating a new header and implementation file for your agent, inheriting from the Turtle class. This new subclass should encapsulate any unique properties and behaviors relevant to the agent type you're introducing.


Integrating New Agents into the World
Incorporating these new agents into the simulation involves several key steps:

Tracking Vector: Add a new vector in the World class to keep track of instances of your new agent type. This vector should store std::shared_ptr to your agents, facilitating easy management and access.



Creation and Registration: When instantiating new agents of your subclass within the World, you must:

Add each new agent to an appropriate Patch.
Store a std::shared_ptr to the new agent in the specific tracking vector you created for its type.
Additionally, add a std::weak_ptr to the agent in the all_turtles vector in the World to keep a general track of all mobile agents, irrespective of their specific types.

Agent Actions and World Interactions
The World class acts as the orchestration layer for the simulation, managing the execution of agent behaviors and environmental interactions. Agent-specific actions, such as movement or executing particular behaviors, are typically facilitated by methods within the World class, which:

Action Execution: For actions requiring direct interaction with the simulation environment (e.g., moving an agent), the World provides methods that take pointers (or smart pointers) to the agents. These methods manipulate the simulation state, ensuring that agents interact with the environment and each other according to the simulation rules.

Agent and Environment Interactions: Methods in the World can modify both the properties of individual agents and environmental attributes stored in Patch objects. This dual interaction capability allows for a dynamic simulation environment where agents and their surroundings influence one another.

