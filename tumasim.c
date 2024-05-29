
#include <assert.h>  // assert
#include <stdbool.h> // bool, true, false
#include <stddef.h>
#include <stdio.h>  // printf, fseek, fopen, fclose, ftell, fread
#include <stdlib.h> // calloc, malloc, free, perror,
#include <string.h> // memcpy, strlen, strdup, strtok, strncmp, strchr

bool debug = false;

typedef enum { STOP, LEFT, RIGHT } PointerMove;

const PointerMove L = LEFT;
const PointerMove R = RIGHT;
const PointerMove S = STOP;

struct Rule {
  char in_value;
  char out_value;
  struct TMState *target_state;
  PointerMove direction;
};

struct TMState {
  char name;
  size_t rules_count;
  struct Rule *rules;
};

// Turing Machine
struct TM {
  struct TMState *initial_state;
  struct TMState **all_states;
  struct TMState **accepting_states;
};

struct RunningTM {
  const struct TM *const machine;
  char *const tape;
  size_t pointer_position;
  struct TMState *current_state;
  size_t tape_buf_len;
};

typedef struct TMState TMState;
typedef struct Rule Rule;
typedef struct TM TM;
typedef struct RunningTM RunningTM;

void print_rule(const Rule *rule) {
  if (rule == NULL) {
    printf("Rule is NULL\n");
    return;
  }

  printf("Rule:\n");
  printf("  In Value: '%c' (%i)\n", rule->in_value, rule->in_value);
  printf("  Out Value: '%c' (%i)\n", rule->out_value, rule->out_value);
  printf("  Target State: %c\n",
         rule->target_state ? rule->target_state->name : ' ');
  printf("  Direction: %s\n", rule->direction == L
                                  ? "LEFT"
                                  : (rule->direction == R ? "RIGHT" : "STOP"));
}
char *read_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Could not open file");
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = malloc(length + 1);
  if (!buffer) {
    perror("Could not allocate memory");
    exit(EXIT_FAILURE);
  }

  fread(buffer, 1, length, file);
  buffer[length] = '\0';

  fclose(file);
  return buffer;
}

TMState *find_state(TMState **states, char name) {
  for (int i = 0; states[i] != NULL; i++) {
    if (states[i]->name == name) {
      return states[i];
    }
  }
  return NULL;
}

void parse_input(const char *input, TM *machine) {
  char *input_copy = strdup(input);
  char *saveptr1, *saveptr2;
  char *line = strtok_r(input_copy, "\n", &saveptr1);

  TMState **all_states = malloc(100 * sizeof(TMState *));
  int state_count = 0;

  if (strncmp(line, "DEBUG ", 6) == 0) {
    debug = line[6] == '1';
    line = strtok_r(NULL, "\n", &saveptr1); // Move to the next line
  }

  // Read all_states from the first line
  char *state_token = strtok_r(line, ", ", &saveptr2);
  while (state_token) {
    TMState *state = malloc(sizeof(TMState));
    state->name = state_token[0];
    state->rules_count = 0;
    state->rules = malloc(10 * sizeof(Rule));
    all_states[state_count++] = state;
    if (debug) {
      printf("Found state [%c]\n", state->name);
    }
    state_token = strtok_r(NULL, ", ", &saveptr2);
  }
  all_states[state_count] = NULL;

  line = strtok_r(NULL, "\n", &saveptr1); // Move to the next line

  TMState **accepting_states = malloc(100 * sizeof(TMState *));
  int accepting_count = 0;
  char initial_state = 0;

  while (line) {
    if (debug) {
      printf("line: [%s]\n", line);
    }
    if (strncmp(line, "init", 4) == 0) {
      initial_state = line[5];
    } else if (strncmp(line, "accept", 6) == 0) {
      char *tok = strtok_r(line + 7, ", ", &saveptr2);
      while (tok) {
        accepting_states[accepting_count++] = find_state(all_states, tok[0]);
        tok = strtok_r(NULL, ", ", &saveptr2);
      }
    } else if (strchr(line, ':')) {
      char state_name = line[0];
      TMState *state = find_state(all_states, state_name);
      char *rule_line = strtok_r(NULL, "\n", &saveptr1);
      while (rule_line && rule_line[0] == '\t') {
        if (debug) {
          printf("rule_line: [%s]\n", rule_line);
        }
        Rule rule;
        rule.in_value = rule_line[1];
        rule.out_value = rule_line[4];
        rule.direction =
            rule_line[7] == '>' ? R : (rule_line[7] == '<' ? L : S);
        rule.target_state = find_state(all_states, rule_line[10]);

        rule.in_value = rule.in_value == '_' ? 0 : rule.in_value;
        rule.out_value = rule.out_value == '_' ? 0 : rule.out_value;

        state->rules[state->rules_count++] = rule;
        rule_line = strtok_r(NULL, "\n", &saveptr1);
        if (debug) {
          printf("New rule in state %c: \n", state->name);
          print_rule(&rule);
        }
      }
    }
    line = strtok_r(NULL, "\n", &saveptr1);
  }

  machine->initial_state = find_state(all_states, initial_state);
  machine->all_states = all_states;
  machine->accepting_states = accepting_states;

  free(input_copy);
}

bool is_accepting_state(const TM *const machine, const TMState *const state) {
  TMState *checked_state;
  size_t i = 0;

  if (state == NULL) {
    return false;
  }

  while (1) {
    checked_state = machine->accepting_states[i++];
    if (checked_state == NULL) {
      break;
    }
    if (checked_state->name == state->name) {
      return true;
    }
  };

  return false;
}

void print_string_as_int_codes(const char *str) {
  if (str == NULL) {
    printf("Invalid string.\n");
    return;
  }

  while (*str != '\0') {
    printf("%d ", (int)(*str));
    str++;
  }

  printf("\n");
}

void print_tape(const RunningTM *const machine) {
  // TODO make it print multiple words separated with 0s
  const char empty_cell_symbol = '_';
  size_t curr_pos = 0;
  size_t printed_chars = 0;
  int pointer_column = -2;
  int first_printed = -1;
  putchar(empty_cell_symbol);
  while (curr_pos < machine->tape_buf_len) {
    const char curr_char = machine->tape[curr_pos];
    if (curr_char != 0) {
      if(first_printed == -1){
        first_printed = curr_pos;
      }
      putchar(curr_char);
      printed_chars++;
      if (machine->pointer_position == curr_pos) {
        pointer_column = printed_chars;
      }
    }
    curr_pos++;
  }
  
  putchar(empty_cell_symbol);
  putchar('\n');
  if (pointer_column == -2) {
    pointer_column = printed_chars + 1;
  }
  if (first_printed - 1 == (int) machine->pointer_position) {
    pointer_column = -1;
  }
  for (int i = 0; i < pointer_column ; ++i) {
    putchar(' ');
  }
  putchar('^');
}

void print_state(const RunningTM *const machine) {
  printf("\nCurrent state: [%c]\n", machine->current_state->name);
}

char get_current_value(const RunningTM *const machine) {
  return machine->tape[machine->pointer_position];
}

const Rule *get_matching_rule(const RunningTM *const machine,
                              const char value) {
  const Rule *const rules = machine->current_state->rules;

  const Rule *current_rule = NULL;
  for (size_t i = 0; i < machine->current_state->rules_count; ++i) {
    current_rule = &rules[i];
    if (current_rule->in_value == value) {
      return current_rule;
    }
  }

  return NULL;
}

PointerMove process_rule(RunningTM *const machine, const Rule *const rule) {
  assert(machine->tape[machine->pointer_position] == rule->in_value);
  machine->current_state = rule->target_state;
  machine->tape[machine->pointer_position] = rule->out_value;
  switch (rule->direction) {
  case LEFT: {
    machine->pointer_position -= 1;
    break;
  }
  case RIGHT: {
    machine->pointer_position += 1;
    break;
  }
  default: {
    // noop
  }
  }

  return rule->direction;
}

PointerMove step_machine(RunningTM *const machine) {
  const char current_value = get_current_value(machine);
  const Rule *activated_rule = get_matching_rule(machine, current_value);

  if (activated_rule == NULL) {
    // Stop the machine
    return STOP;
  }

  return process_rule(machine, activated_rule);
}

TMState simulate_machine(const TM *const machine, const char *input,
                         size_t input_length) {

  const size_t tape_size = 50000;
  const int tape_initial_offset = 10000;

  char *const tape_buffer = calloc(tape_size, sizeof(char));

  TMState final_state;

  // Copy input into tape
  memcpy(tape_buffer + tape_initial_offset, input, input_length);

  RunningTM running_machine = {machine, tape_buffer, tape_initial_offset,
                               machine->initial_state, 50000};

  PointerMove next_move;
  do {
    print_state(&running_machine);
    print_tape(&running_machine);
    getchar();
    next_move = step_machine(&running_machine);
  } while (next_move != STOP);

  final_state = *running_machine.current_state;

  print_tape(&running_machine);
  putchar('\n');

  free(tape_buffer);

  return final_state;
}

bool check_accept(TM const *machine, const char *input,
                  const size_t input_length) {
  const TMState final_state = simulate_machine(machine, input, input_length);

  printf("Final state: %c\n", final_state.name);

  return is_accepting_state(machine, &final_state);
}

int main(const int argc, const char **argv) {
  // TM should accept 11*0*1
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <code_file> <machine_input>\n", argv[0]);
    return EXIT_FAILURE;
  }

  char *code = read_file(argv[1]);

  TM machine;
  parse_input(code, &machine);

  if (debug) {
    printf("Initial state: %c\n", machine.initial_state->name);
    printf("Accepting states: ");
    for (int i = 0; machine.accepting_states[i] != NULL; i++) {
      printf("%c ", machine.accepting_states[i]->name);
    }
    printf("\n");

    print_string_as_int_codes(argv[2]);
  }

  bool accepted = check_accept(&machine, argv[2], strlen(argv[2]));

  putchar('\n');
  printf(accepted ? "ACCEPTED" : "REJECTED");
  putchar('\n');

  // Cleanup
  for (int i = 0; machine.all_states[i] != NULL; i++) {
    free(machine.all_states[i]->rules);
    free(machine.all_states[i]);
  }
  free(machine.all_states);
  free(machine.accepting_states);
  free(code);

  return 0;
}
