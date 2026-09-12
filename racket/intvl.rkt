#lang racket
(require rackunit)
(provide (all-defined-out))


(define-struct interval (low high left-open? right-open?) #:transparent)
;; An Interval is one of:
;; * false
;; * (make-interval Num Num Bool Bool)
;;   Requires: low <= high
;;
;; Examples:
;; (make-interval 3 5 true true) => (3,5)
;; (make-interval 3 5 true false) => (3,5]
;; (make-interval 3 5 false false) => [3,5]
;; (make-interval 5 3 false true) => false


;; Positive and Negative Infinity
(define +inf (expt 2 32))
(define -inf (- +inf))
;; Number Sets
(define real (make-interval -inf +inf true true))
(define pos (make-interval 0 +inf true true))
(define neg (make-interval -inf 0 true true))
(define nonpos (make-interval -inf 0 true false))
(define nonneg (make-interval 0 +inf false true))


;; (valid-interval? intvl) produces true if intvl represents a valid interval
;;   and false otherwise.
;; valid-interval?: Interval -> Bool
(define (valid-interval? intvl)
  (and (interval? intvl)
       (or (and (not (interval-left-open? intvl))
                (not (interval-right-open? intvl))
                (<= (interval-low intvl) (interval-high intvl)))
           (< (interval-low intvl) (interval-high intvl)))))

;; Tests:
(check-equal? (valid-interval? (make-interval 0 5 true true)) true)     ; (0,5)
(check-equal? (valid-interval? (make-interval 0 5 true false)) true)    ; (0,5]
(check-equal? (valid-interval? (make-interval 0 5 false true)) true)    ; [0,5)
(check-equal? (valid-interval? (make-interval 0 5 false false)) true)   ; [0,5]
(check-equal? (valid-interval? (make-interval 5 5 true true)) false)    ; (5,5)
(check-equal? (valid-interval? (make-interval 5 5 true false)) false)   ; (5,5]
(check-equal? (valid-interval? (make-interval 5 5 false true)) false)   ; [5,5)
(check-equal? (valid-interval? (make-interval 5 5 false false)) true)   ; [5,5]
(check-equal? (valid-interval? (make-interval 10 5 true true)) false)   ; (10,5)
(check-equal? (valid-interval? (make-interval 10 5 true false)) false)  ; (10,5]
(check-equal? (valid-interval? (make-interval 10 5 false true)) false)  ; [10,5)
(check-equal? (valid-interval? (make-interval 10 5 false false)) false) ; [10,5]


;; (in-interval? n intvl) produces true if n is in the interval intvl
;;   or false otherwise.
;; in-interval?: Num Interval -> Bool
(define (in-interval? n intvl)
  (and (valid-interval? intvl)
       (cond [(interval-left-open? intvl)
              (> n (interval-low intvl))]
             [else (>= n (interval-low intvl))])
       (cond [(interval-right-open? intvl)
              (< n (interval-high intvl))]
             [else (<= n (interval-low intvl))])))

;; Tests:
(check-equal? (in-interval? 5 real) true)
(check-equal? (in-interval? 5 pos) true)
(check-equal? (in-interval? -5 neg) true)
(check-equal? (in-interval? 5 neg) false)
(check-equal? (in-interval? 0 neg) false)
(check-equal? (in-interval? 0 nonneg) true)
(check-equal? (in-interval? 0 false) false)


;; (intersection i1 i2) produces an interval where i1 and i2 intersect.
;; intersection: Interval Interval -> Interval
(define (intersection i1 i2)
  (cond [(or (not (valid-interval? i1)) (not (valid-interval? i2))) false]
        [else
         (local
           [(define low (max (interval-low i1) (interval-low i2)))
            (define high (min (interval-high i1) (interval-high i2)))
            (define left-open?
              (cond [(> (interval-low i1) (interval-low i2))
                     (interval-left-open? i1)]
                    [(= (interval-low i1) (interval-low i2))
                     (or (interval-left-open? i1) (interval-left-open? i2))]
                    [else (interval-left-open? i2)]))
            (define right-open?
              (cond [(< (interval-high i1) (interval-high i2))
                     (interval-right-open? i1)]
                    [(= (interval-high i1) (interval-high i2))
                     (or (interval-right-open? i1) (interval-right-open? i2))]
                    [else (interval-right-open? i2)]))
            (define intvl (make-interval low high left-open? right-open?))]
    
           (cond [(valid-interval? intvl) intvl]
                 [else false]))]))

;; Tests:
;; (137/5,+inf) (-inf,274/9) -> (274/9,137/5)
(check-equal? (intersection (make-interval 137/5 +inf true true)
                            (make-interval -inf 274/9 true true))
              (make-interval 137/5 274/9 true true))
;; (-137/5,+inf) (-274/9,+inf) -> (-137/5,+inf)
(check-equal? (intersection (make-interval -137/5 +inf true true)
                            (make-interval -274/9 +inf true true))
              (make-interval -137/5 +inf true true))
;; (-inf,-137/5) (-inf,-274/9) -> (-inf,-274/9)
(check-equal? (intersection (make-interval -inf -137/5 true true)
                            (make-interval -inf -274/9 true true))
              (make-interval -inf -274/9 true true))
;; (-inf,137/5) (274/9,+inf) -> false
(check-equal? (intersection (make-interval -inf 137/5 true true)
                            (make-interval 274/9 +inf true true))
              false)

;; Same lower bound, different upper bound
;; (3,4) [3,5] -> (3,4)
(check-equal? (intersection (make-interval 3 4 true true)
                            (make-interval 3 5 false false))
              (make-interval 3 4 true true))
;; [3,4] (3,5] -> (3,4]
(check-equal? (intersection (make-interval 3 4 false false)
                            (make-interval 3 5 true false))
              (make-interval 3 4 true false))
;; [3,4) [3,5] -> [3,4)
(check-equal? (intersection (make-interval 3 4 false true)
                            (make-interval 3 5 false false))
              (make-interval 3 4 false true))

;; Different lower bound, same upper bound
;; [5,7) (6,7) -> (6,7)
(check-equal? (intersection (make-interval 5 7 false true)
                            (make-interval 6 7 true true))
              (make-interval 6 7 true true))
;; (5,7] [6,7] -> [6,7]
(check-equal? (intersection (make-interval 5 7 true false)
                            (make-interval 6 7 false false))
              (make-interval 6 7 false false))
;; (5,7) [6,7) -> [6,7)
(check-equal? (intersection (make-interval 5 7 true true)
                            (make-interval 6 7 false true))
              (make-interval 6 7 false true))

;; Interval of length 0
;; [5,5] [5,5] -> [5,5]
(check-equal? (intersection (make-interval 5 5 false false)
                            (make-interval 5 5 false false))
              (make-interval 5 5 false false))

;; Invalid intervals
;; (7,5) [6,6] -> false
(check-equal? (intersection (make-interval 7 5 true true)
                            (make-interval 6 6 false false))
              false)
;; (5,5] [5,5) -> false
(check-equal? (intersection (make-interval 5 5 true false)
                            (make-interval 5 5 false true))
              false)
;; [5,5] (5,5) -> false
(check-equal? (intersection (make-interval 5 5 false false)
                            (make-interval 5 5 true true))
              false)
;; false (8,11] -> false
(check-equal? (intersection false (make-interval 8 11 true false))
              false)
;; (1,2) false -> false
(check-equal? (intersection (make-interval 1 2 true true) false)
              false)


;; (int-interval intvl) produces a closed interval of integers in intvl.
;; int-interval: Interval -> Interval
(define (int-interval intvl)
  (cond [(not (valid-interval? intvl)) false]
        [else
         (local
           [(define low (interval-low intvl))
            (define high (interval-high intvl))]
           
           (make-interval
            (cond [(and (interval-left-open? intvl)
                        (integer? low) (not (= low -inf)))
                   (+ low 1)]
                  [else (ceiling low)])
            (cond [(and (interval-right-open? intvl)
                        (integer? high) (not (= high +inf)))
                   (- high 1)]
                  [else (floor high)])
            (= low -inf) (= high +inf)))]))

;; Tests:
(check-equal? (int-interval (make-interval 137/5 274/9 true true))
              (make-interval 28 30 false false))
(check-equal? (int-interval (make-interval 137/5 -274/9 true true))
              false)
(check-equal? (int-interval (make-interval -137/5 274/9 true true))
              (make-interval -27 30 false false))
(check-equal? (int-interval (make-interval -274/9 -137/5 true true))
              (make-interval -30 -28 false false))
(check-equal? (int-interval (make-interval 27 31 true true))
              (make-interval 28 30 false false))
(check-equal? (int-interval (make-interval 27 31 true false))
              (make-interval 28 31 false false))
(check-equal? (int-interval (make-interval 27 31 false true))
              (make-interval 27 30 false false))
(check-equal? (int-interval (make-interval 27 31 false false))
              (make-interval 27 31 false false))
(check-equal? (int-interval (make-interval 5 5 true true))
              false)
(check-equal? (int-interval (make-interval 5 5 false false))
              (make-interval 5 5 false false))
(check-equal? (int-interval false) false)


;; (interval->string var intvl) produces the string representation of
;;   the domain of var in the interval intvl.
;; interval->string: Str Interval -> Str
;; Requires: var must not be an empty string
(define (interval->string var intvl)
  (cond [(not (valid-interval? intvl)) "Invalid Interval!"]
        [else
         (local
           [(define low (interval-low intvl))
            (define high (interval-high intvl))]
           
           (cond [(and (= low -inf) (= high +inf))
                  (string-append var " is in the real number set")]
                 [(= low -inf)
                  (string-append var
                                 (cond [(interval-right-open? intvl) "<"]
                                       [else "<="])
                                 (number->string high))]
                 [(= high +inf)
                  (string-append var
                                 (cond [(interval-left-open? intvl) ">"]
                                       [else ">="])
                                 (number->string low))]
                 [(= low high)
                  (string-append var "=" (number->string low))]
                 [else
                  (string-append (number->string low)
                                 (cond [(interval-left-open? intvl) "<"]
                                       [else "<="])
                                 var
                                 (cond [(interval-right-open? intvl) "<"]
                                       [else "<="])
                                 (number->string high))]))]))

;; Tests:
(check-equal? (interval->string "n" false) "Invalid Interval!")
(check-equal? (interval->string "n" (make-interval 5 3 true false))
              "Invalid Interval!")
(check-equal? (interval->string "n" pos) "n>0")
(check-equal? (interval->string "n" nonpos) "n<=0")
(check-equal? (interval->string "n" (make-interval 5 5 false false)) "n=5")
(check-equal? (interval->string "n" (make-interval 3 5 true true)) "3<n<5")
(check-equal? (interval->string "n" (make-interval -3 5 false true)) "-3<=n<5")
(check-equal? (interval->string "n" (make-interval -5 -3 true false)) "-5<n<=-3")
(check-equal? (interval->string "x" (make-interval -274/9 -137/5 false false))
              "-274/9<=x<=-137/5")


;; (string->interval str) produces an interval based on the
;;   string representation str.
;; string->interval: Str -> Interval
;; Requires:
;;   str must be in one of the following forms:
;;     * (a,b)
;;     * (a,b]
;;     * [a,b)
;;     * [a,b]
;;   where a and b are real numbers.
(define (string->interval str)
  (local
    [(define str-no-space (string-replace str " " ""))
     (define str-comp (string-split str-no-space ","))
     (define first-half (first str-comp))
     (define second-half (second str-comp))
     (define second-end (- (string-length second-half) 1))

     (define low (string->number (substring first-half 1)))
     (define high (string->number (substring second-half 0 second-end)))
     (define left-open? (char=? (string-ref first-half 0) #\())
     (define right-open? (char=? (string-ref second-half second-end) #\)))]

    (make-interval low high left-open? right-open?)))

;; Tests:
(check-equal? (string->interval "(7,9)")
              (make-interval 7 9 true true))
(check-equal? (string->interval " (7, 9]  ")
              (make-interval 7 9 true false))
(check-equal? (string->interval "[ 0  . 07 0, 9  0. 0 ) ")
              (make-interval 0.070 90.0 false true))
(check-equal? (string->interval " [-9 , - 7]  ")
              (make-interval -9 -7 false false))
