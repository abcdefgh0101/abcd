:- dynamic(has_symptom/1).

start :-
    write('--- Medical Diagnosis Expert System ---'), nl,
    ask_questions,
    diagnose(Disease),
    write('Diagnosis: You may have '), write(Disease), nl,
    cleanup.

ask_questions :-
    symptom_list(Symptoms),
    ask_all(Symptoms).

symptom_list([fever, cough, sore_throat, body_ache, 
              sneezing, runny_nose, chills, sweating, 
              nausea, headache]).

ask_all([]).
ask_all([Symptom|Rest]) :-
    format('Do you have ~w? (yes/no) ', [Symptom]),
    read(Reply),
    ((Reply == yes) -> assert(has_symptom(Symptom)) ; true),
    ask_all(Rest).

diagnose('COVID-19') :-
    has_symptom(fever),
    has_symptom(cough),
    has_symptom(headache),
    has_symptom(body_ache), !.

diagnose(flu) :-
    has_symptom(fever),
    has_symptom(cough),
    has_symptom(body_ache),
    has_symptom(sore_throat), !.

diagnose(common_cold) :-
    has_symptom(sneezing),
    has_symptom(runny_nose),
    has_symptom(cough), !.

diagnose(malaria) :-
    has_symptom(fever),
    has_symptom(chills),
    has_symptom(sweating),
    has_symptom(nausea), !.

diagnose(strep_throat) :-
    has_symptom(sore_throat),
    has_symptom(fever),
    \+ has_symptom(cough), !.

diagnose(unknown).

cleanup :-
    retractall(has_symptom(_)).