#include <cstring>
#include <libqalculate/qalculate.h>
#include "QalculateNasc.h"
#include <memory>
#include <thread>
#include <algorithm>

std::shared_ptr<Calculator> calc;
MathStructure *mstruct;

extern "C" {
    void new_calculator() {
        // set locale of the system
        setlocale (LC_ALL, "");
        calc = std::shared_ptr<Calculator> (new Calculator() );
        calc->loadGlobalDefinitions();
        calc->loadLocalDefinitions();
        load_currencies ();
        calc->useIntervalArithmetic(false);
    }
	
    void add_function (const char* name, const char* expr) {
	   std::string  name_string (name);
	    std::string expr_string (expr);
	    calc->addFunction (new UserFunction ("",name_string,expr_string));
    }

    void load_currencies () {
        calc->loadGlobalCurrencies();
        calc->loadExchangeRates();
    }

    int get_function_size () {
        return calc->functions.size();
    }

    int get_variable_size() {
        return calc->variables.size();
    }

    char* get_function_name (int index) {
        if (calc->functions[index]->isActive() ) {
            const ExpressionName *ename_r;
            ename_r = &calc->functions[index]->preferredInputName (false, true, false, false);
            std::string str = ename_r->name;
            return convert_string (str);
        } else {
            return convert_string ("");
        }
    }

    char* get_function_description (int index) {
        if (calc->functions[index]->isActive() ) {
            std::string str = calc->functions[index]->description ();
            return convert_string (str);
        } else {
            return convert_string ("");
        }

    }

    char* get_function_category (int index) {
        if (calc->functions[index]->isActive() ) {
            std::string str = calc->functions[index]->category ();
            return convert_string (str);
        } else {
            return convert_string ("");
        }

    }

    char* get_function_title (int index) {
        if (calc->functions[index]->isActive() ) {
            std::string str = calc->functions[index]->title ();
            return convert_string (str);
        } else {
            return convert_string ("");
        }

    }

    char* get_function_arguments (int index) {
        if (calc->functions[index]->isActive() ) {
            std::string str = "";
            Argument *arg;
            int args = calc->functions[index]->maxargs();
            if (args < 0) {
                args = calc->functions[index]->minargs() + 1;
            }
            for (int i = 1; i <= args; i++) {
                arg = calc->functions[index]->getArgumentDefinition (i);
                if (arg) {
                    arg = arg->copy();
                    str += arg->name();
                    if (i != args) { str += ";"; }
                }
            }
            transform (str.begin(), str.end(), str.begin(), (int (*) (int) ) tolower);
            return convert_string (str);
        } else { return convert_string (""); }
    }

    char* get_function_arguments_list (int index) {
        if (calc->functions[index]->isActive() ) {
            std::string str = "";
            Argument *arg;
            int args = calc->functions[index]->maxargs();
            if (args < 0) {
                args = calc->functions[index]->minargs() + 1;
            }
            for (int i = 1; i <= args; i++) {
                arg = calc->functions[index]->getArgumentDefinition (i);
                if (arg) {
                    arg = arg->copy();
                    std::string str_arg = arg->printlong();
                    str += arg->name();
                    str += ": ";
                    str += str_arg;
                    if (i != args) { str += "\n"; }
                }
            }
            transform (str.begin(), str.end(), str.begin(), (int (*) (int) ) tolower);
            return convert_string (str);
        } else {
            return convert_string ("");
        }
    }

    char* get_variable_name (int index) {
        if (calc->variables[index]->isActive() ) {
            const ExpressionName *ename_r;
            ename_r = &calc->variables[index]->preferredInputName (false, true, false, false);
            std::string str = ename_r->name;
            return convert_string (str);
        } else {
            return convert_string ("");
        }
    }

    char* get_variable_category (int index) {
        if (calc->variables[index]->isActive() ) {
            std::string str = calc->variables[index]->category ();
            return convert_string (str);
        } else {
            return convert_string ("");
        }
    }

    char* get_variable_title (int index) {
        if (calc->variables[index]->isActive() ) {
            std::string str = calc->variables[index]->title ();
            return convert_string (str);
        } else {
            return convert_string ("");
        }
    }

    char* Calculator_calculate (const char* input) {
        std::string in (input);
        std::string result_str  = "";
        try {
            result_str = intern_calc_terminate (in);
        } catch (std::exception) {
            result_str  = "";
        }
        return convert_string (result_str);
    }

    char* Calculator_calculate_store_variable (const char* input, const char* variable_char) {
        std::string in (input);
        std::string var (variable_char);
        std::string result_str  = "";
        try {
            result_str = intern_calc_terminate (in);

            char * buffer = convert_string (result_str);

            // stored variables must have . decimal sign
            int len = result_str.length();
            for (int i = 0; i < len; i++) {
                if (result_str[i] == ',') {
                    result_str[i] = '.';
                }
            }
            KnownVariable *variable = nullptr;
            if (calc->variableNameTaken (var, variable) ) {
                //var exist, edit it
                variable = (KnownVariable*) calc->getActiveVariable (var);
                variable->setLocal (true);
                variable->set (result_str);
            } else {
                //var dont exist create it
                variable = new KnownVariable ("Temporary", var, result_str, var, true);
                calc->addVariable (variable);
            }
            return buffer;
        } catch (std::exception) {
            result_str  = "error";
            return convert_string (result_str);
        }
    }

    void clear_variables() {
        calc->resetVariables();
    }

    void abortt() {
        calc->abort();
    }

    void delete_calculator() {
        calc.reset();
    }

    int busy() {
        if (calc->busy () )
        { return 1; }
        else
        { return 0; }
    }

    char* get_exchange_rates_url () {
        return convert_string (calc->getExchangeRatesUrl () );
    }

    char* get_exchange_rates_filename () {
        return convert_string (calc->getExchangeRatesFileName () );
    }
}

char* convert_string (std::string str) {
    char * buffer = new char[str.length() + 1];
    std::copy (str.c_str(), str.c_str() + str.length() + 1, buffer);
    return buffer;
}
bool hasEnding(std::string mainStr, std::string toMatch) {
    auto it = toMatch.begin();
	return mainStr.size() >= toMatch.size() &&
			std::all_of(std::next(mainStr.begin(),mainStr.size() - toMatch.size()), mainStr.end(), [&it](const char & c){
				return ::tolower(c) == ::tolower(*(it++))  ;
	} );
}

PrintOptions getPrintOptions(std::string input) {
    PrintOptions printops;
    printops.multiplication_sign = MULTIPLICATION_SIGN_ASTERISK;
    printops.number_fraction_format = FRACTION_DECIMAL;
    printops.max_decimals = 9;
    printops.use_max_decimals = true;
    printops.use_unicode_signs = true;
    printops.use_unit_prefixes = false;

    //conversions
    if(hasEnding(input,"to hex")){
        printops.base = BASE_HEXADECIMAL;
    }else if(hasEnding(input,"to bin")){
        printops.base = BASE_BINARY;
    }else if(hasEnding(input,"to dec")){
        printops.base = BASE_DECIMAL;
    }else if(hasEnding(input,"to oct")){
        printops.base = BASE_OCTAL;
    }else if(hasEnding(input,"to duo")){
        printops.base = BASE_DUODECIMAL;
    }else if(hasEnding(input,"to roman")){
        printops.base = BASE_ROMAN_NUMERALS;
    }else if(hasEnding(input,"to bijective")){
        printops.base = BASE_BIJECTIVE_26;
    }else if(hasEnding(input,"to roman")){
        printops.base = BASE_ROMAN_NUMERALS;
    }else if(hasEnding(input,"to sexa")){
        printops.base = BASE_SEXAGESIMAL;
    }else if(hasEnding(input,"to fp32")){
        printops.base = BASE_FP32;
    }else if(hasEnding(input,"to fp64")){
        printops.base = BASE_FP64;
    }else if(hasEnding(input,"to fp16")){
        printops.base = BASE_FP16;
    }else if(hasEnding(input,"to fp80")){
        printops.base = BASE_FP80;
    }else if(hasEnding(input,"to fp128")){
        printops.base = BASE_FP128;
    }else if(hasEnding(input,"to time")){
        printops.base = BASE_TIME;
    }else if(hasEnding(input,"to unicode")){
        printops.base = BASE_UNICODE;
    }
    //TODO do real time zone converstions
    else if(hasEnding(input,"to utc")||hasEnding(input,"to gmt")){
		printops.time_zone = TIME_ZONE_UTC;
    } else if(hasEnding(input,"to cet")){
		printops.time_zone = TIME_ZONE_CUSTOM;
		printops.custom_time_zone = 60;
    }
    
    return printops;
}

std::string intern_calc_wait (std::string input) {
    while (calc->busy() ) {
        std::chrono::milliseconds duration (10);
        std::this_thread::sleep_for (duration);
    }
    return intern_calc (input);
}

std::string intern_calc_terminate (std::string input) {
    if (calc->busy() ) {
        std::cout << "qalc abort" << std::endl;
        calc->abort();
    }
    return intern_calc (input);
}

std::string intern_calc (std::string input) {
    calc->beginTemporaryStopMessages();
    EvaluationOptions evalops;
    //evalops.parse_options.angle_unit = ANGLE_UNIT_RADIANS;
    evalops.parse_options.unknowns_enabled = false;
    evalops.allow_complex = false;
    evalops.structuring = STRUCTURING_SIMPLIFY;
    //evalops.auto_post_conversion = POST_CONVERSION_BEST;
    evalops.keep_zero_units = false;
    std::string res = calc->unlocalizeExpression (input, evalops.parse_options);

    mstruct = new MathStructure();
    bool finished = calc->calculate (mstruct, res, 5000, evalops);
    if (!finished)
    { return "calculation takes too long"; }

    // if calculation has a error return a empty string. Maybe return a error message?
    int warnings_count;
    bool had_errors = calc->endTemporaryStopMessages (NULL, &warnings_count) > 0;
    if (had_errors) { return ""; }

    
    PrintOptions printops = getPrintOptions(input);
    mstruct->format (printops);
    std::string result_str = mstruct->print (printops);

    delete mstruct;
    return result_str;//calc->localizeExpression (result_str);
}
