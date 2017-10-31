#ifndef OLUCT_POLICY_HPP_
#define OLUCT_POLICY_HPP_

#include <environment.hpp>
#include <policy/uct.hpp>

/**
 * @brief OLUCT policy
 */
class oluct {
public:
    std::vector<std::vector<double>> action_space; ///< Full action space
    unsigned decision_criterion_selector;
    environment * envt; ///< Pointer to an environment, used for action space reduction
    uct pl;

    /**
     * @brief Constructor
     *
     * Constructu using the given parameters
     * @param {const parameters &} p; parameters
     * @param {environment *} en; pointer to the environment, used for action space reduction
     */
    oluct(const parameters &p, environment *en) :
        pl(p,en)
    {
        action_space = p.ACTION_SPACE;
        decision_criterion_selector = p.DECISION_CRITERION_SELECTOR;
    }

    /**
     * @brief Reduced action space
     *
     * Compute the action space available at the given state.
     * @param {const std::vector<double &} s; state
     * @return Return the reduced action space.
     */
    std::vector<std::vector<double>> reduced_action_space(const std::vector<double> &s) {
        std::vector<std::vector<double>> ras;
        for(auto &a : action_space) {
            std::vector<double> s_p;
            if(envt->state_transition(s,a,s_p)) {
                ras.push_back(a);
            }
        }
        return ras;
    }

    /**
     * @brief Switch on decision criterion
     *
     * Select the decision criterion according to the chosen algorithm.
     * The tested tree is the current tree saved in the parameters.
     * @param {const std::vector<double> &} s; the current state of the agent
     * @return Return 'true' if the sub-tree is kept.
     */
    bool decision_criterion(const std::vector<double> & s) {
        (void) s;//TODO: remove with other decision criteria
        switch(decision_criterion_selector) {
            default: { // Plain decision criterion
                return true;
            }
        }
    }

    /**
     * @brief Policy operator
     *
     * Policy operator for the undertaken action at given state.
     * @param {const std::vector<double> &} s; given state
     * @return Return the undertaken action at s.
     */
	std::vector<double> operator()(const std::vector<double> &s) {
        if(!pl.root_node.is_fully_expanded() || !decision_criterion(s)) {
            pl.build_uct_tree(s);
        }
        std::vector<double> ra = pl.get_recommended_action(pl.root_node);
        pl.root_node.move_to_child(pl.argmax_score(pl.root_node),s);
        return ra;
	}

    /**
     * @brief Process reward
     *
     * Process the resulting reward from transition (s,a,s_p)
     * @param {std::vector<double> &} s; state
     * @param {std::vector<double> &} a; action
     * @param {std::vector<double> &} s_p; next state
     */
    void process_reward(
        const std::vector<double> & s,
        const std::vector<double> & a,
        const std::vector<double> & s_p)
    {
        /* OLUCT policy does not learn. */
        (void) s;
        (void) a;
        (void) s_p;
    }
};

#endif // OLUCT_POLICY_HPP_
