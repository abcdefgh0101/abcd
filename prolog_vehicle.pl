/* 
   Vehicle Classification System
   - Takes user input to classify vehicles
   - Covers motorcycles, cars, trucks, buses, bicycles, and more
*/

:- initialization(main).

main :-
    write('=== VEHICLE CLASSIFICATION SYSTEM ==='), nl, nl,
    write('Answer the following questions about the vehicle:'), nl, nl,
    
    % Get vehicle attributes
    get_wheels(Wheels),
    get_handlebars(HasHandlebars),
    get_passenger_capacity(Passengers),
    get_cargo_capacity(Cargo),
    get_passenger_vehicle(CarriesPassengers),
    
    % Classify the vehicle
    ( classify_vehicle(VehicleType, Wheels, HasHandlebars, Passengers, Cargo, CarriesPassengers) ->
        format('\nClassification Result: This is a ~w\n', [VehicleType])
    ; 
        write('\nUnable to classify this vehicle with the given information.\n')
    ).

% Classification rules
classify_vehicle(motorcycle, 2, yes, _, _, _).
classify_vehicle(bicycle, 2, yes, 1, low, no).
classify_vehicle(car, 4, no, _, low, yes).
classify_vehicle(truck, 6, _, _, high, _).
classify_vehicle(bus, 4, _, Passengers, _, _) :- Passengers > 10.
classify_vehicle(pickup_truck, 4, _, _, medium, no).
classify_vehicle(scooter, 2, yes, 1, low, no).
classify_vehicle(semi_truck, 18, _, _, high, no).

% Input predicates
get_wheels(Wheels) :-
    repeat,
    write('1. How many wheels does it have? '),
    read(Wheels),
    ( integer(Wheels), Wheels > 0 -> true
    ; write('Invalid input. Please enter a positive number.\n'), fail
    ).

get_handlebars(HasHandlebars) :-
    repeat,
    write('2. Does it have handlebars? (yes/no) '),
    read(HasHandlebars),
    ( member(HasHandlebars, [yes, no]) -> true
    ; write('Please answer with "yes." or "no."\n'), fail
    ).

get_passenger_capacity(Passengers) :-
    repeat,
    write('3. What is the passenger capacity? '),
    read(Passengers),
    ( integer(Passengers), Passengers >= 0 -> true
    ; write('Invalid input. Please enter a non-negative number.\n'), fail
    ).

get_cargo_capacity(Cargo) :-
    repeat,
    write('4. What is the cargo capacity? (low/medium/high) '),
    read(Cargo),
    ( member(Cargo, [low, medium, high]) -> true
    ; write('Please answer with "low.", "medium.", or "high."\n'), fail
    ).

get_passenger_vehicle(CarriesPassengers) :-
    repeat,
    write('5. Is it primarily for carrying passengers? (yes/no) '),
    read(CarriesPassengers),
    ( member(CarriesPassengers, [yes, no]) -> true
    ; write('Please answer with "yes." or "no."\n'), fail
    ).