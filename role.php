<?php

function _simplesamlphp_auth_evaulaterolerule($roleruleevaluation, $attributes) {

    _simplesaml_auth_debug('Evaluate rule (key=' . $roleruleevaluation[0] . ',operator=' . $roleruleevaluation[1] . ',value=' . $roleruleevaluation[2] . ')');

    if (!array_key_exists($roleruleevaluation[0], $attributes)) return FALSE;
    $attribute = $attributes[$roleruleevaluation[0]];

    switch ($roleruleevaluation[1]) {
        case '=' :
            return in_array($roleruleevaluation[2], $attribute);

        case '@=' :
            $dc = explode('@', $attribute[0]);
            if (count($dc) != 2) return FALSE;
            return ($dc[1] == $roleruleevaluation[2]);
    }

    return FALSE;
}



function _simplesamlphp_auth_rolepopulation($rolemap) {

    global $_simplesamlphp_auth_as;
    global $_simplesamlphp_auth_saml_attributes;
    $roles = array();

    _simplesaml_auth_debug('Rolemap: ' . $rolemap);

    // Check if valid local session exists..
    if ($_simplesamlphp_auth_as->isAuthenticated()) {
        $attributes = $_simplesamlphp_auth_saml_attributes;

        if (empty($rolemap)) return $roles;

        _simplesaml_auth_debug('Evaluate rolemap: ' . $rolemap);

        $rolerules = explode('|', $rolemap);

        foreach ($rolerules AS $rolerule) {

            _simplesaml_auth_debug('Evaluate role rule: ' . $rolerule);

            $roleruledecompose = explode(':', $rolerule);

            $roleid = $roleruledecompose[0];
            $roleruleevaluations = explode(';', $roleruledecompose[1]);

            $addnew = TRUE;
            foreach ($roleruleevaluations AS $roleruleevaluation) {

                _simplesaml_auth_debug('Evaluate rule evaulation: ' . $roleruleevaluation);

                $roleruleevaluationdc = explode(', ', $roleruleevaluation);
                if (!_simplesamlphp_auth_evaulaterolerule($roleruleevaluationdc, $attributes)) $addnew = FALSE;
            }
            if ($addnew) {
                $roles[$roleid] = $roleid;
                _simplesaml_auth_debug('Add new role: ' . $roleid);
            }

        }
    }
    return $roles;
}
