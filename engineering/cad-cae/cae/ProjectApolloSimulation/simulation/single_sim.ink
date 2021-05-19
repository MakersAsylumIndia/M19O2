inkling "2.0"
using Number

experiment {
    num_workers: "3",
    env_runners_per_sampler: "2"
}

function Reward (gs: GameState) {
    return 1
}

function Terminal (gs: GameState) {
    return false
}

type GameState {
    P: Number,
    T: Number,
    Tw: Number,
    xA: Number,
    xB: Number,
    yA: Number,
    yB: Number
}

type Action {
    real_cycle_time: Number<10 .. 20>,
    vent_time_fract: Number<0.6 .. 0.9>
}

type Config {
    episode_length: -1,
    deque_size: 1
}

simulator ApolloSimulator(action: Action, config: Config): GameState {
}

graph (input: GameState): Action {
    concept Balance(input): Action {
        experiment {
            random_seed: "42"
        }
        curriculum {
            source ApolloSimulator
        }
    }
    output Balance
}