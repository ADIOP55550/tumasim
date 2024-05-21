
#include <assert.h>  // assert
#include <stdbool.h> // bool, true, false
#include <stddef.h>
#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free
#include <string.h> // memcpy, strlen

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
  struct Rule rules[];
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

TMState A, B, C, D, E, F, G;

TMState A = {'A',
             1,
             {
                 (Rule){'1', '1', &B, R},
             }};

TMState B = {'B',
             2,
             {
                 (Rule){'1', '1', &C, R},
                 (Rule){'0', '0', &D, R},
             }};

TMState C = {'C',
             2,
             {
                 (Rule){'0', '0', &D, R},
                 (Rule){'1', '1', &G, R},
             }};

TMState D = {'D', 2, {(Rule){'0', '0', &D, R}, (Rule){'1', '1', &E, R}}};

TMState E = {'E', 1, {(Rule){0, 0, &F, S}}};

TMState F = {'F', 0, {}};

TMState G = {
    'G',
    3,
    {(Rule){'0', '0', &D, R}, (Rule){'1', '1', &G, R}, (Rule){0, 0, &F, S}}};

TMState *all_states[] = {&A, &B, &C, &D, &E, &F, &G, NULL};

TMState *accepting_states[] = {&C, &F, NULL};

TM machine = {&A, all_states, accepting_states};




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
  size_t pos = 0;
  size_t printed_chars = 0;
  int pointer_column = -1;
  while (pos < machine->tape_buf_len) {
    const char curr_char = machine->tape[pos];
    if (curr_char != 0) {
      putchar(curr_char);
      printed_chars++;
      if (machine->pointer_position == pos) {
        pointer_column = printed_chars;
      }
    }
    pos++;
  }

  putchar('\n');
  if (pointer_column == -1) {
    pointer_column = printed_chars + 1;
  }
  for (int i = 0; i < pointer_column - 1; ++i) {
    putchar(' ');
  }
  putchar('^');
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

  char *const tape_buffer = malloc(sizeof(char) * 50000);

  const int tape_initial_offset = 10000;

  TMState final_state;

  // Copy input into tape
  memcpy(tape_buffer + tape_initial_offset, input, input_length);

  RunningTM running_machine = {machine, tape_buffer, tape_initial_offset,
                               machine->initial_state, 50000};

  PointerMove next_move;
  do {
    print_tape(&running_machine);
    getchar();
    next_move = step_machine(&running_machine);
  } while (next_move != STOP);

  final_state = *running_machine.current_state;

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

  if (argc < 2) {
    printf("Please provide input string.\n");
    exit(1);
  }

  print_string_as_int_codes(argv[1]);

  bool accepted = check_accept(&machine, argv[1], strlen(argv[1]));

  printf(accepted ? "ACCEPTED" : "REJECTED");
}
