#ifndef CAL_FORMULA_H
#define CAL_FORMULA_H

#include <stdint.h>
#include "my_struct.h"
#include "auto_zero_adjust.h"

void PHOS_value_calculation2(unn_std_var_typdef *opt_std_vars);
void SUL_value_calculation1(unn_std_var_typdef *opt_std_vars);
void NIT_value_calculation3(unn_std_var_typdef *opt_std_vars);
void NIT_interpolation_calculation3(unn_std_var_typdef *opt_std_vars);
void POT_value_calculation5(unn_std_var_typdef *opt_std_vars);
void OC_value_calculation6(unn_std_var_typdef *opt_std_vars);

#endif /* CAL_FORMULA_H */
