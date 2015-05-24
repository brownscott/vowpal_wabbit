/*
Copyright (c) by respective owners including Yahoo!, Microsoft, and
individual contributors. All rights reserved.  Released under a BSD
license as described in the file LICENSE.
 */
#pragma once
//TODO: extend to handle CSOAA_LDF and WAP_LDF
  LEARNER::base_learner* cb_algs_setup(vw& all);

namespace CB_ALGS {
  template <bool is_learn>
    float get_cost_pred(LEARNER::base_learner* scorer, CB::cb_class* known_cost, example& ec, uint32_t index, uint32_t base)
  {
    CB::label ld = ec.l.cb;

    label_data simple_temp;
    simple_temp.initial = 0.;
    if (known_cost != nullptr && index == known_cost->action)
      {
	simple_temp.label = known_cost->cost;
	simple_temp.weight = 1.;
      }
    else 
      {
	simple_temp.label = FLT_MAX;
	simple_temp.weight = 0.;
      }
    
    ec.l.simple = simple_temp;

    if (is_learn && simple_temp.label != FLT_MAX)
      scorer->learn(ec, index-1+base);
    else
      scorer->predict(ec, index-1+base);
    
    float pred = ec.pred.scalar;
    
    ec.l.cb = ld;

    return pred;
  }

	
}

namespace CB{
	struct cb {
		size_t cb_type;
		uint32_t num_actions;
		COST_SENSITIVE::label cb_cs_ld;
		LEARNER::base_learner* scorer;
		float avg_loss_regressors;
		size_t nb_ex_regressors;
		float last_pred_reg;
		float last_correct_cost;

		cb_class* known_cost;
	};

	
}

float get_unbiased_cost(CB::cb &c, uint32_t action, CB::label& ld);

